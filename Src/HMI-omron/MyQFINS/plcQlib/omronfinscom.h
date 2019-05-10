#ifndef TOMRONFINSFRAME_H
#define TOMRONFINSFRAME_H

#include <string>

#include <QTcpSocket>

#include <QThread>

#include "types.h"

QByteArray n2a4( unsigned int number);
QByteArray n2a3( unsigned int number);
QByteArray n2a2( unsigned int number);
QByteArray n2a1( unsigned int number);

struct FINScommandFlags {
    u8 ICF;
    u8 RSV;
    u8 GCT;
    u8 DNA;
    u8 DA1;
    u8 DA2;
    u8 SNA;
    u8 SA1;
    u8 SA2;
    u8 SID;
    u8 MRC;
    u8 SRC;
    FINScommandFlags() {
        ICF = 0; RSV=0; GCT=0; DNA=0; DA1=0; DA2=0; SNA=0; SA1=0; SA2=0; SID=0; MRC=0; SRC=0;
    }
    QByteArray getRaw() {
        QByteArray dest(12,'\x00');
        dest[ 0] = ICF;
        dest[ 1] = RSV;
        dest[ 2] = GCT;
        dest[ 3] = DNA;
        dest[ 4] = DA1;
        dest[ 5] = DA2;
        dest[ 6] = SNA;
        dest[ 7] = SA1;
        dest[ 8] = SA2;
        dest[ 9] = SID;
        dest[10] = MRC;
        dest[11] = SRC;
        return dest;
    }
};

struct sFinsHeader {
    u32 length;
    u32 command;
    u32 errorCode;
    sFinsHeader() { length = 0; command =0; errorCode =0;}
    QByteArray getRaw() {
        QByteArray dest;
        dest.append( "FINS");
        dest.append( n2a4( length));
        dest.append( n2a4( command));
        dest.append( n2a4( errorCode));
        return dest;
    }
};


QString getFinsErrorString( u32 error);

struct sFinsResponseFlags {
    bool frameError; //!< bad frame format (all data in structure are invalid)
    bool responseParsed; //!< true if flags are set according reponse
    u32 finsError; //!< Fins frame error
    u8  MRC;  //!< response command[0]
    u8  SRC;  //!< response command[1]
    u8  MRES; //!< response status[0]
    u8  SRES; //!< response status[1]
    u8  SID;  //!< response SID
    u8  DA1;  //!< response node addr
    u8  SA1;  //!< response server addr
    sFinsResponseFlags() {
        frameError = true;
        responseParsed = false;
        finsError=0;
        MRC=0; SRC=0; MRES=0; SRES=0; SID=0; DA1=0; SA1=0;
    }
};

class TomronFinsCommandFrame
{
public:
    TomronFinsCommandFrame( u8 serverAdr, u8 clientAdr, u8 MRC, u8 SRC, const QByteArray &data, u8 SID=0);
    TomronFinsCommandFrame();
    QByteArray getRaw();
private:
    FINScommandFlags cmdFrameFlags;
    QByteArray cmdFrameData;
};



class OmronFinsFrame
{
public:
    OmronFinsFrame( sFinsHeader header, TomronFinsCommandFrame cmdf);
    OmronFinsFrame( sFinsHeader header, QByteArray data);
    OmronFinsFrame( QByteArray data, int dataLen);
    OmronFinsFrame( void);
    ~OmronFinsFrame( );

    QByteArray getRaw();

private:
    QByteArray finsFrameRaw; //fins frame buffer
    int finsFrameRawLength;
    sFinsHeader finsFrameHeader;

    TomronFinsCommandFrame commandFrame;
};


//common function for Omron PLCs
QByteArray adrToFinsAddr( QString addr,int *bytesPerElement=0);

class OmronPlc : public QObject
{
    Q_OBJECT

public:
    OmronPlc( QString plcIP, quint16 port=9600, QObject *parent=0);

    //Q_Object Virtuals
    ~OmronPlc();
    OmronPlc();

    bool plcIsAlive();
    void setSimulMode( bool);

public slots:
    int  open( void);
    void close( void);
    //int  readData(  QString addr, u16 &data); //!< \brief Read data from PLC
    int  readData(  QString addr, QVector<u16> &dataa); //!< \brief Read data from PLC
    int  writeData( QString addr, u16  value) {
        QList<u16> data;
        data.append( value);
        return writeData(addr,data);
    }
    int  writeData( QString addr, QList<u16> data); //!< \brief Write data to PLC

private slots:

signals:
    //void openDone( void);
    //void closeDone( void);
    //void responseRecieved( TomronFinsCommandFrame& response);

private:
    bool opened;
    bool simulMode;
    QTcpSocket *plcSocket;
    QString plcIPaddr;
    quint16 plcIPport;
    u8 clientNodeNo;
    u8 serverNodeNo;
    u8 commandSeqCounter;

    //functions
    int recieveFinsFrame( QByteArray &dest);
    int sendAndRecieveFinsFrame( QByteArray &src, QByteArray &dest, int maxRetry=3);
    int sendCommand( u8 MRC, u8 SRC, const QByteArray& data=QByteArray());
    sFinsResponseFlags decodeFinsFlags( const QByteArray& frame);
    int checkFinsResponse( const QByteArray& commandFrame, const QByteArray& responseFrame);
    QString finsToHumanFormat( const QByteArray& frame);

    //Q_Object Virtuals
    bool event(QEvent *) {return false;}
    bool eventFilter ( QObject * , QEvent * ) { return false;}
    //const QMetaObject * metaObject () const
};

#endif // TOMRONFINSFRAME_H
