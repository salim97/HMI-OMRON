#include <string>
#include <map>
#include <cstring>
#include <QtDebug>
#include <QRegExp>
#include <QStringList>
#include <QtNetwork>

#include <QThread>

using namespace std;

#include "omronfinscom.h"

//#define WRITEDATA_PERFORMANCE_DEBUG
//#define READDATA_PERFORMANCE_DEBUG
//#define READDATAARRAY_PERFORMANCE_DEBUG

QByteArray n2a4( unsigned int number)
{
    QByteArray outa("\00\00\00\00",4);
    outa[0] = ( (number >> 24) & 0xff);
    outa[1] = ( (number >> 16) & 0xff);
    outa[2] = ( (number >>  8) & 0xff);
    outa[3] = ( (number >>  0) & 0xff);
    return outa;
}

QByteArray n2a3( unsigned int number)
{
    QByteArray outa("\00\00\00",3);
    outa[0] = ( (number >> 16) & 0xff);
    outa[1] = ( (number >>  8) & 0xff);
    outa[2] = ( (number >>  0) & 0xff);
    return outa;
}

QByteArray n2a2( unsigned int number)
{
    QByteArray outa("\00\00",2);
    outa[0] = ( (number >>  8) & 0xff);
    outa[1] = ( (number >>  0) & 0xff);
    return outa;
}

QByteArray n2a1( unsigned int number)
{
    QByteArray outa("\00",1);
    outa[0] = ( (number >>  0) & 0xff);
    return outa;
}

QString getFinsErrorString( u32 error)
{
    //init fins Error structure
    QString errString;
    switch ( error) {
            case 0x0000000: errString = "No"; break;
            case 0x0000001: errString = "The header is not FINS (ASCII code)"; break;
            case 0x0000002: errString = "The data length is too long."; break;
            case 0x0000003: errString = "The command is not supported."; break;
            case 0x0000020: errString = "All connections are in use."; break;
            case 0x0000021: errString = "The specified node is already connected."; break;
            case 0x0000022: errString = "Attempt to access a protected node from an unspecified IP"; break;
            case 0x0000023: errString = "The client FINS node address is out of range."; break;
            case 0x0000024: errString = "The same FINS node address is being used by the client and server."; break;
            case 0x0000025: errString = "All the node addresses available for allocation have been used."; break;
            default: errString = "Unknown Error Code";
    }
    return errString;
}


QByteArray adrToFinsAddr( QString addrString, int *bytesPerElement)
{
    bool parsedOk = true;

    int  bytes = 0;

    QRegExp rx;
    rx.setPattern("(.)([0-9]*):?([0-9]*)" ); //D103:00 W512

    int pos = rx.indexIn( addrString);

    QStringList list = rx.capturedTexts();

    u8 memcode;   // memory code from datasheet table
    u8 bitAddr=0; // bit number, zero for word addres

    u32 addr;
    addr = list[2].toUInt( &parsedOk);

    if ( list[3] != QString()) {
        //address is bite
        bitAddr = list[3].toUInt( &parsedOk);
        bytes = 1;
        switch ((char) list[1][0].toLatin1()) {
            case 'C': memcode = 0x30; break;
            case 'W': memcode = 0x31; break;
            case 'H': memcode = 0x32; break;
            case 'A': memcode = 0x33; break;
            case 'D': memcode = 0x02; break;
            default: parsedOk = false; memcode = 0x00;
        }
    } else {
        //address is word
        bytes = 2;
        switch ((char) list[1][0].toLatin1()) {
            case 'C': memcode = 0xB0; break;
            case 'W': memcode = 0xB1; break;
            case 'H': memcode = 0xB2; break;
            case 'A': memcode = 0xB3; break;
            case 'D': memcode = 0x82; break;
            default: parsedOk = false; memcode = 0x00;
        }
    }

    //Create memspec packet:
    //   [memory code - 1yte][begin addr - 3byte]
    QByteArray memSpec("");
    QByteArray debug( n2a2(addr));
    if( parsedOk) {
        memSpec.append( memcode);
        memSpec.append( n2a2(addr));
        memSpec.append( n2a1(bitAddr));
    } else {
        bytes = 0;
    }
    if( bytesPerElement) {
        *bytesPerElement = bytes;
    }
    /*
    qDebug() << "           Addr " << addrString
             << QString(" %1 %2 %3 %4 ").arg((unsigned char)memSpec[0])
                                        .arg((unsigned char)memSpec[1])
                                        .arg((unsigned char)memSpec[2])
                                        .arg((unsigned char)memSpec[3]);
    */
    return memSpec;
}


//------------------------------
//Omron Fins Command Frame
//------------------------------

TomronFinsCommandFrame::TomronFinsCommandFrame()
{
}

TomronFinsCommandFrame::TomronFinsCommandFrame( u8 serverAdr, u8 clientAdr, u8 MRC, u8 SRC, const QByteArray &data, u8 SID)
{
    cmdFrameFlags.ICF = 0x80;
    cmdFrameFlags.GCT = 0x02;
    cmdFrameFlags.DA1 = serverAdr;
    cmdFrameFlags.SA1 = clientAdr;
    cmdFrameFlags.MRC = MRC;
    cmdFrameFlags.SRC = SRC;
    cmdFrameFlags.SID = SID;
    cmdFrameData = QByteArray();
    cmdFrameData.append( data);
}

//! \return Raw data
QByteArray TomronFinsCommandFrame::getRaw()
{
    QByteArray dest("");

    QByteArray flag("");
    QByteArray data("");

    flag = cmdFrameFlags.getRaw();
    data = cmdFrameData;

    dest.append( flag);
    dest.append( data);

    return dest;
}

//------------------------------
//Omron Fins (TCP) Frame
//------------------------------

//! Construct frame from Fins command frame (this is usualy way fro FIMS commands)
OmronFinsFrame::OmronFinsFrame( sFinsHeader header, TomronFinsCommandFrame cmdf)
{
    finsFrameHeader = header;
    commandFrame = cmdf;

    //set frame length
    int cmdDataLength = commandFrame.getRaw().length();
    finsFrameHeader.length = cmdDataLength + 8;

    //store raw frame
    finsFrameRaw = finsFrameHeader.getRaw();
    finsFrameRaw.append( commandFrame.getRaw());

}

//! Construct Fins control frame ( for Fins node address obtain ...)
OmronFinsFrame::OmronFinsFrame( sFinsHeader header, QByteArray data)
{
    finsFrameHeader = header;
    finsFrameHeader.length = data.length() + 8;

    //store raw frame
    //copy header to rawFrame
    finsFrameRaw = finsFrameHeader.getRaw();
    //copy data do rawFrame
    finsFrameRaw.append( data);

}

OmronFinsFrame::OmronFinsFrame( void)
{
}

OmronFinsFrame::~OmronFinsFrame( )
{
}

QByteArray OmronFinsFrame::getRaw()
{
    return finsFrameRaw;
}

OmronPlc::~OmronPlc()
{

}

sFinsResponseFlags OmronPlc::decodeFinsFlags( const QByteArray &response)
{
    sFinsResponseFlags flags;

    if( response.length() < 16 ) {
        flags.frameError = true;
        return flags;
    } else {
        //Check frame strcture (FINS and lengths)
        int fLength      = (unsigned char)(response.at(4))*(256*256*256)
                         + (unsigned char)(response.at(5))*(256*2256)
                         + (unsigned char)(response.at(6))*(256)
                         + (unsigned char)(response.at(7));
        if( (fLength != (response.length() - 8))
            || response[0] != 'F'
            || response[1] != 'I'
            || response[2] != 'N'
            || response[3] != 'S'
            ) {
            flags.frameError = true;
            return flags;
        }
        flags.frameError = false;
    };

    flags.finsError =  (unsigned char)(response.at(12))*(256*256*256)
                     + (unsigned char)(response.at(13))*(256*256)
                     + (unsigned char)(response.at(14))*(256)
                     + (unsigned char)(response.at(15));
    if( response.length() > 27) {
        flags.MRC  = (unsigned char)(response.at( 16+10));
        flags.SRC  = (unsigned char)(response.at( 16+11));
        flags.SID  = (unsigned char)(response.at( 16+9));
        flags.DA1  = (unsigned char)(response.at( 16+4));
        flags.SA1  = (unsigned char)(response.at( 16+7));
    };
    if( response.length() > 29) {
        flags.MRES = (unsigned char)(response.at( 16+12));    //response code
        flags.SRES = (unsigned char)(response.at( 16+13));
        flags.responseParsed =true;
    };
    return flags;
}

QString OmronPlc::finsToHumanFormat( const QByteArray &frame)
{
    QString str;
    for( int i=0; i<frame.length(); i++) {
        switch (i) {
            case  0: str += QString( QChar(frame.at(i))); break; //F
            case  1: str += QString( QChar(frame.at(i))); break; //I
            case  2: str += QString( QChar(frame.at(i))); break; //N
            case  3: str += QString( QChar(frame.at(i))); break; //S

            case  4: str += QString( "\n [len]  ");
            case  5:
            case  6:
            case  7: str += QString( "%1").arg( (uchar)frame.at(i), 2, 16, QChar('0')); break;

            case  8: str += QString( "\n [cmd]  ");
            case  9:
            case 10:
            case 11: str += QString( "%1").arg( (uchar)frame.at(i), 2, 16, QChar('0')); break;

            case 12: str += QString( "\n [err]  ");
            case 13:
            case 14:
            case 15: str += QString( "%1").arg( (uchar)frame.at(i), 2, 16, QChar('0')); break;

            case 16: str += QString( "\n ICF: %1\n").arg( (uchar)frame.at(i), 2, 16, QChar('0')); break;
            case 17: str += QString( " RSV: %1\n").arg( (uchar)frame.at(i), 2, 16, QChar('0')); break;
            case 18: str += QString( " GCT: %1\n").arg( (uchar)frame.at(i), 2, 16, QChar('0')); break;
            case 19: str += QString( " DNA: %1\n").arg( (uchar)frame.at(i), 2, 16, QChar('0')); break;
            case 20: str += QString( " DA1: %1\n").arg( (uchar)frame.at(i), 2, 16, QChar('0')); break;
            case 21: str += QString( " DA2: %1\n").arg( (uchar)frame.at(i), 2, 16, QChar('0')); break;
            case 22: str += QString( " SNA: %1\n").arg( (uchar)frame.at(i), 2, 16, QChar('0')); break;
            case 23: str += QString( " SA1: %1\n").arg( (uchar)frame.at(i), 2, 16, QChar('0')); break;
            case 24: str += QString( " SA2: %1\n").arg( (uchar)frame.at(i), 2, 16, QChar('0')); break;
            case 25: str += QString( " SID: %1\n").arg( (uchar)frame.at(i), 2, 16, QChar('0')); break;
            case 26: str += QString( " MRC: %1\n").arg( (uchar)frame.at(i), 2, 16, QChar('0')); break;
            case 27: str += QString( " SRC: %1\n").arg( (uchar)frame.at(i), 2, 16, QChar('0')); break;
            default:
                     str += QString( " %1: %2\n")
                                .arg( (ushort)i, 3)
                                .arg( (uchar)frame.at(i), 2, 16, QChar('0'));
        }
    }
    return str;
}

//! simple check for errors
//! \return Zero if response has no error
int OmronPlc::checkFinsResponse( const QByteArray &commandFrame, const QByteArray &responseFrame)
{
    //Check if frame response has no error
    //check if response frame is response to command frame (address, command, sid pass)

    //decode send frame
    sFinsResponseFlags sFlags;
    sFlags = decodeFinsFlags( commandFrame);

    //decode response frame
    sFinsResponseFlags rFlags;
    rFlags = decodeFinsFlags( responseFrame);

    if( rFlags.frameError) {
        qDebug() << "Frame Error detected"
                 << "\n Command:"
                 << finsToHumanFormat(commandFrame)
                 << "\n Response"
                 << finsToHumanFormat(responseFrame);
        return -1;
    };

    if( rFlags.finsError >0) {
        qDebug() << "Fins Error response detected"
                 << "\n Command:"
                 << finsToHumanFormat(commandFrame)
                 << "\n Response"
                 << finsToHumanFormat(responseFrame);
        return -2;
    };

    /** \todo check only for FIMNS command response not with establish communication commands
      * now working because MRES and SRES are set to 0 on initialization
      */
    if( rFlags.MRES > 0 || rFlags.SRES > 0) {
        qDebug() << QString("Fins Command Error response detected MRES:%1 SRES:%2").arg(rFlags.MRES).arg(rFlags.SRES)
                 << "\n Command:"
                 << finsToHumanFormat(commandFrame)
                 << "\n Response"
                 << finsToHumanFormat(responseFrame);
        return -2;
    };


    /** \todo check only for FINS command response not with establish communication commands
      * now working because SID is set to 0 on initialization
      */
    if( rFlags.SID != sFlags.SID) {
        qDebug() << QString("Fins command Sequence Error response detected sendSEQ:%1 recSEQ:%2").arg(sFlags.SID).arg(rFlags.SID)
                 << "\n Command:"
                 << finsToHumanFormat(commandFrame)
                 << "\n Response"
                 << finsToHumanFormat(responseFrame);
        return -2;
    };

    return 0;
}


OmronPlc::OmronPlc( QString plcIP, quint16 port, QObject *parent)
        :QObject( parent)
{
    qDebug() << "TomronPLC( QString plcIP, quint16 port, QObject *parent)" << parent << this->thread();
    plcIPaddr = plcIP;
    plcIPport = port;
    opened = false;
    simulMode = false;
    clientNodeNo = 0;
    serverNodeNo = 0;
    plcSocket = NULL;
}

/** Recieve fins frame
  * added to support fragmented packed
  */
int OmronPlc::recieveFinsFrame(QByteArray &dest)
{
    int s;
    QByteArray response,header;
    QTime t; t.start();
    bool timeout=false;
    //wait for fins header
    int i=0, ba;
    do {
        plcSocket->waitForReadyRead( 100);
        ba = plcSocket->bytesAvailable();
        if( i++ > 30) timeout = true;
    } while (  ba < 8 && !timeout);

    if( timeout) return -1;
    //read header
    response = plcSocket->read( 8);
    header = response;
    //calculate fins frame length
    int fLength      = (unsigned char)(response.at(4))*(256*256*256)
                     + (unsigned char)(response.at(5))*(256*256)
                     + (unsigned char)(response.at(6))*(256)
                     + (unsigned char)(response.at(7));
    //wait for rest of fins frame
    do {
        plcSocket->waitForReadyRead( 100);
        ba = plcSocket->bytesAvailable();
        if( i++ > 30) timeout = true;
    } while (  ba < fLength && !timeout);

    //read response (or uncoplette)
    response.append( plcSocket->readAll());
    if( timeout) return -1;
    //read rest of frame

    if( response.length() != (fLength+8)) {
        qWarning() << "!PLC read more data that expected";
    }
    dest = response;
    return 0;
}

int OmronPlc::sendAndRecieveFinsFrame(QByteArray &src, QByteArray &dest, int maxRetry)
{
    if( !plcSocket) return -1;

    int r,sendTryies = 0;
    bool s;
    SendFrame:
    plcSocket->write( src);
    s = plcSocket->waitForBytesWritten( 1000);
    if ( s != true) {
        qWarning() << "Can't write! [" << plcIPaddr << "]";
    } else {
        //recieve response
        r = recieveFinsFrame( dest);
        if ( r != 0) {
            qWarning() << "Can't read - timeout! [" << plcIPaddr << "]";
        }
    }

    //! \todo Validate response packet flags against errors and packet type
    if( r!=0 || s!=true || checkFinsResponse( src, dest) != 0) {
        qWarning() << "PLC bad FINS response " << finsToHumanFormat(dest);
        if( ++sendTryies < maxRetry) {
            //try again...
            qWarning() << "PLC  sendAndRecieveFinsFrame - retry ";
            //read corrupted data (if any) and resend query
            plcSocket->readAll();
            goto SendFrame;
        } else {
            //bad response - no addition retry;
            qWarning() << "Plc comm failed ... bad response " << plcIPaddr;
            return -1;
        }
    }
    return 0;
}

int OmronPlc::open(void)
{
    if( simulMode) return 0;

    bool r;
    QByteArray response;

    //Open socket connection
    qDebug() << "TomronPLC::open(void) Open plcSocket";
    if( !plcSocket) {
        plcSocket = new QTcpSocket( );
    }
    qDebug() << "TomronPLC::open(void) Connect To host";
    plcSocket->connectToHost( plcIPaddr, plcIPport);
    r = plcSocket->waitForConnected(1000);
    if ( !r) {
        qDebug() << "Not Connected! [" << plcIPaddr << "]";
        opened = false;
        return -1;
    }

    //Format frame - get auto address (see Section 7-4 FINS/TCP Method)
    sFinsHeader header;
    header.command      = 0x000000;
    header.errorCode    = 0x000000;
    QByteArray dataf( "\00\00\00\00", 4);
    OmronFinsFrame finsSendFrame( header, dataf);

    //send frame
    QByteArray temp( "");
    temp.append( finsSendFrame.getRaw());

    int connectTryies = 0;
    SendFrame:
    int s;
    s = sendAndRecieveFinsFrame( temp, response, 1);

    //! \todo Validate response packet flags against errors and packet type
    if( s != 0) {
        //error in response
        if( ++connectTryies < 5) {
            qWarning() << "PLC open - retry bad FINS response";
            //read corrupted data (if any) and resend query
            plcSocket->readAll();
            goto SendFrame;
        } else {
            goto abortOpen;
        }
    } else {
        //response frame is OK
        //check if response has valid format (command code = 10)
        if( response.at(7) != 0x10 && response.length()>23) {
            qWarning() << "PLC open - unexpected response " << finsToHumanFormat(response);
            if( ++connectTryies < 5) {
                qWarning() << "PLC open - retry ";
                //read corrupted data (if any) and resend query
                plcSocket->readAll();
                goto SendFrame;
            } else {
                goto abortOpen;
            }
        };
        //decode response
        clientNodeNo = response[19];
        serverNodeNo = response[23];

        if( clientNodeNo == 0 || serverNodeNo == 0) {
            if( ++connectTryies < 5) {
                qWarning() << "PLC open - retry bad FINS addres";
                //read corrupted data (if any) and resend query
                plcSocket->readAll();
                goto SendFrame;
            } else {
                goto abortOpen;
            }
        }
        //succesfull open
        //! \todo Implement try command - get PLC type
        opened = true;
        return 0;
    }
    abortOpen:
    qWarning() << "Plc open failed ... " << plcIPaddr;
    close();
    opened = false;
    return -1;
}

bool OmronPlc::plcIsAlive( )
{
    //! \todo Provide more sophisticated test !
    return this->opened;
}

void OmronPlc::setSimulMode( bool goSimul)
{
    //! \todo Provide check if simul is caled to running PLC !!

    this->simulMode = goSimul;
}

/*
int  OmronPlc::writeData( QString addr, u16 data)
{
    #ifdef WRITEDATA_PERFORMANCE_DEBUG
    QTime t; t.start(); //debug elapsed time metering
    #endif

    if( simulMode) {
        qDebug() << "Simul write! [" << addr << "," << data << "]";
        return 0;
    }
    if( !opened) return -1;

    QByteArray cmdData;
    int bytesPerElement;
    cmdData.append( adrToFinsAddr( addr, &bytesPerElement));
    cmdData.append( n2a2( 1));                 // number of data

    switch( bytesPerElement ) {
        case 1: cmdData.append( n2a1( data)); break;// data itself
        case 2: cmdData.append( n2a2( data)); break;
        default:
            qDebug() << QString("Addres conversion failed for WriteData to  ") << addr;
            return 1; //addres conversion failed
    }

    //Memory write FINS command 0x01,0x02
    TomronFinsCommandFrame cmdFrame( serverNodeNo, clientNodeNo, 01, 02, cmdData , ++commandSeqCounter);
    sFinsHeader finsHeader;
    finsHeader.command = 0x02;
    OmronFinsFrame finsTcpFrame( finsHeader, cmdFrame);

    if( !plcSocket) return -1;
    //send data to PLC
    QByteArray temp = finsTcpFrame.getRaw();
    plcSocket->write( temp);
    while(!plcSocket->waitForBytesWritten( 20));
    while(!plcSocket->waitForReadyRead( 20));
    QByteArray response = plcSocket->readAll();

    #ifdef WRITEDATA_PERFORMANCE_DEBUG
    qDebug() << QString( "Time elapsed: TomronPLC::writeData( QString %1, u16 %2) %3 ms").arg(addr).arg(data).arg( t.elapsed());
    #endif
    return 0;   //OK
} */


int  OmronPlc::writeData( QString addr, QList<u16> data)
{
    #ifdef WRITEDATA_PERFORMANCE_DEBUG
    QTime t; t.start(); //debug elapsed time metering
    #endif

    if( simulMode) {
        qDebug() << "Simul write! [" << addr << ",{" << data << "}]";
        return 0;
    }
    if( !opened)    return -1;
    if( !plcSocket) return -1;

    QByteArray cmdData;
    int bytesPerElement;
    cmdData.append( adrToFinsAddr( addr, &bytesPerElement));
    cmdData.append( n2a2( data.length()));                 // number of data

    if( bytesPerElement != 1 && bytesPerElement != 2) {
        qWarning() << QString("Addres conversion failed for WriteData to  ") << addr;
        return -1; //addres conversion failed
    }

    for( int i=0; i<data.size(); i++) {
        switch( bytesPerElement ) {
            case 1: cmdData.append( n2a1( data[i])); break;// data itself
            case 2: cmdData.append( n2a2( data[i])); break;
        }
    }

    //Memory write FINS command 0x01,0x02
    TomronFinsCommandFrame cmdFrame( serverNodeNo, clientNodeNo, 01, 02, cmdData , ++commandSeqCounter);
    sFinsHeader finsHeader;
    finsHeader.command = 0x02;
    OmronFinsFrame finsTcpFrame( finsHeader, cmdFrame);

    //send data to PLC
    QByteArray temp = finsTcpFrame.getRaw();
    if( temp.length() >= 1023) {
        qWarning() << "PLC write fail - Maximum packet length for " << addr;
        return -1;
    }

    //send data to PLC and recieve response
    QByteArray response;
    int s;
    s = sendAndRecieveFinsFrame( temp, response);

    //check comm status
    if( s != 0) {
        qWarning() << "Bad write response from " << this->plcIPaddr;
        return -2;
    }

    #ifdef WRITEDATA_PERFORMANCE_DEBUG
    qDebug() << QString( "Time elapsed: TomronPLC::writeData( QString %1, u16 %2) %3 ms").arg(addr).arg(data).arg( t.elapsed());
    #endif
    return 0;   //OK
}

/*
int  OmronPlc::readData( QString addr, u16 &data)
{
    #ifdef READDATA_PERFORMANCE_DEBUG
    QTime t; t.start(); //debug elapsed time metering
    #endif
    if( simulMode) {
        data = 0;
        qDebug() << "Simul read! [" << addr << "]";
        return 0;
    }
    if( !opened) return -1;

    QByteArray cmdData;
    int bytesPerElement;
    cmdData.append( adrToFinsAddr( addr, &bytesPerElement));
    cmdData.append( n2a2( 1));                 // number of data
    if( bytesPerElement == 0) {
        qDebug() << QString("Addres conversion failed for readData to  ") << addr;
        return 1; //addres conversion failed
    };

    //Memory read FINS command 0x01,0x01
    TomronFinsCommandFrame cmdFrame( serverNodeNo, clientNodeNo, 01, 01, cmdData , ++commandSeqCounter);
    sFinsHeader finsHeader;
    finsHeader.command = 0x02;
    OmronFinsFrame finsTcpFrame( finsHeader, cmdFrame);

    if( !plcSocket) return -1;
    //send data to PLC
    QByteArray temp = finsTcpFrame.getRaw();
    plcSocket->write( temp);
    while (!plcSocket->waitForBytesWritten( 20));
    while (!plcSocket->waitForReadyRead( 20));
    QByteArray response = plcSocket->readAll();

    u16 respi;
    if( bytesPerElement == 1) {
        //bite read
        respi = ((unsigned char)(response.at(30)));
    } else {
        //byte read
        respi = ((unsigned char)(response.at(30)))*256 + ((unsigned char)(response.at(31)));
    }
    data = respi;

    #ifdef READDATA_PERFORMANCE_DEBUG
    qDebug() << QString( "Time elapsed: readData( QString %1, u16 %2) %3 ms").arg(addr).arg(data).arg( t.elapsed());
    #endif

    return 0;   //OK
}*/

int  OmronPlc::readData(  QString addr, QVector<u16> &dataa)
{
    #ifdef READDATAARRAY_PERFORMANCE_DEBUG
    QTime t; t.start(); //debug elapsed time metering
    #endif

    if( simulMode) {
        dataa.fill( qrand(), dataa.size());
        qDebug() << "Simul vector read! [" << addr << "," << dataa.size() << "]";
        return 0;
    }
    if( !opened)    return -1;
    if( !plcSocket) return -1;

    QByteArray cmdData;
    int bytesPerElement;
    int length = dataa.size();
    cmdData.append( adrToFinsAddr( addr, &bytesPerElement));
    cmdData.append( n2a2( length));                 // number of data

    //Memory read FINS command 0x01,0x01
    TomronFinsCommandFrame cmdFrame( serverNodeNo, clientNodeNo, 01, 01, cmdData , ++commandSeqCounter);
    sFinsHeader finsHeader;
    finsHeader.command = 0x02;
    OmronFinsFrame finsTcpFrame( finsHeader, cmdFrame);

    //send data to PLC and recieve response
    QByteArray temp = finsTcpFrame.getRaw();
    QByteArray response;
    int s;
    s = sendAndRecieveFinsFrame( temp, response);

    //check comm status
    if( s != 0) {
        qWarning() << "Bad read response from " << this->plcIPaddr;
        return -2;
    }

    //check response length
    int res_len = response.length();
    if( length*2 + 30 != res_len) {
        qWarning() << "Bad read response length from " << this->plcIPaddr;
        return -2;
    }
    //read and decode data
    for( int i = 0; i<length; i++) {
        u16 respi = ((unsigned char)(response.at(30 + i*2)))*256 + ((unsigned char)(response.at(31 + i*2)));
        dataa[i] = respi;
    }

    #ifdef READDATAARRAY_PERFORMANCE_DEBUG
    qDebug() << QString( "Time elapsed: readData( QString %1, u16 %2) %3 ms").arg(addr).arg(dataa.size() ).arg( t.elapsed());
    #endif

    return 0;   //OK
}

//! Close connection
void OmronPlc::close( void)
{
    opened = false;

    if( !plcSocket) return; //already closed...

    /* Not working - response code is ERROR...
    //Write FINC connection close command
    //In direct connect socket close is sufficient, but with SSH tunel there connection
    //persist and there is problem with renew address.
    sFinsHeader header;
    header.command      = 0x002730;
    header.errorCode    = 0x000000;
    QByteArray dataf( "\00\00\00\00", 4);
    OmronFinsFrame finsSendFrame( header, dataf);

    //send frame
    QByteArray temp( "");
    temp.append( finsSendFrame.getRaw());
    plcSocket->write( temp);
    plcSocket->waitForBytesWritten(1000);
    plcSocket->waitForReadyRead( 1000);
    QByteArray response = plcSocket->readAll( );
    qDebug() << "PLC close response " << finsToHumanFormat( response);
    */

    plcSocket->close();  /** \todo This couse problem why ??? */
    delete plcSocket; plcSocket = NULL;
    return;
}
