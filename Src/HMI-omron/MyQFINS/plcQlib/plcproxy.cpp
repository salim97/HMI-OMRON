
#include <QMessageBox>
#include <QTime>

#include "plcproxy.h"

#define MAX_ENABLED_READ_LEN (450)
//#define PLC_PROXY_DEBUG

PlcProxy::PlcProxy( QString plcIPaddr,  QObject *parent)
        :QThread( parent), quit( false)
{
    this->plcIPaddr = plcIPaddr;
    qDebug() << "tPlcProxy( QString plcIPaddr,  QObject *parent)" << parent << this->thread();
    plc = NULL;

    qRegisterMetaType< u16>("u16");  //register for emit signal
    qRegisterMetaType< QList<u16> >("QList<u16>");
    qRegisterMetaType< PlcProxy::EplcProxyError>("PlcProxy::EplcProxyError");

    //clear proxy memory
    for( long i=0; i<PROXY_MEMORY_SIZE;i++) {
        proxyMemory[i] = 0x00;
    }
}

PlcProxy::~PlcProxy()
{
    stopProxy();
}

/**
  * Warning Thic cannot be called if Proxy is running !!!
  * \todo Add Semaphore here
  */
bool PlcProxy::testConnection()
{
    if( plcOpen() !=0) return false;

    bool result=true;

    QVector<u16> dummy(1);
    if( plc->readData("A0",dummy) != 0) result = false;
    plcClose();

    return result;
}

int PlcProxy::plcOpen()
{
    if( plc) {
        //already open - close and destroy
        plc->close();
        delete plc; plc=NULL;
    }
    plc = new OmronPlc( plcIPaddr, 9600, 0);   //! \todo Why baseclas must be 0, Error Child cannot be create for parent in another thread

    //open PLC
    if( plc) {
        int s;
        s = plc->open();
        if( s) {
            //! \todo add action when PLC failed, nofity owner ....
            //plc->setSimulMode( true);
            return -1;
        }
    } else {
        //plc create fail
        return -2;
    }
    return 0;
}

void PlcProxy::plcClose()
{
    if( plc) {
        //if open - close and destroy
        plc->close();
        delete plc; plc = NULL;
    }
}

bool PlcProxy::plcIsOk()
{
    if( plc) {
        return plc->plcIsAlive();
    }
    return false;
}

//! Write value, no callback signal
void PlcProxy::writeData( QString addr, QList<u16> data)
{
    wq_type qe;
    qe.addr = addr;
    qe.data = data;

    writeQueue_lock.lockForWrite();
    writeQueue.enqueue( qe );
    writeQueue_lock.unlock();
}

QList<u16> float2omronData( double value, int length)
{
    QList<u16> data;
    union fct {
        double v_double;
        float  v_float;
        u16 v_u16[10];
    } fcu;
    switch( length) {
    case 2:
        fcu.v_float = value;
        data.append( fcu.v_u16[0]);
        data.append( fcu.v_u16[1]);
        break;
    case 4:
        fcu.v_double = value;
        data.append( fcu.v_u16[0]);
        data.append( fcu.v_u16[1]);
        data.append( fcu.v_u16[2]);
        data.append( fcu.v_u16[3]);
        break;
    default:
        qWarning() << "Bad conversion length float2omronData";
    }
    return data;
}

QList<u16> value2BcdOmronData( long int value, int length)
{
    QList<u16> data;
    long int remain, digit1, digit2, digit3, digit4;
    remain = value;
    for( int i=0; i<length; i++) {

        digit1  = remain % 10;
        remain /= 10;

        digit2  = remain % 10;
        remain /= 10;

        digit3  = remain % 10;
        remain /= 10;

        digit4  = remain % 10;
        remain /= 10;

        data.append( (digit4<<12) + (digit3<<8) + (digit2<<4) + digit1);
    }
    if( remain > 0) {
        //somthing remain after conversion, - not all written to PLC
        qWarning() << "Number to BCD conversion probably fail for [" << value << ",Len:" << length << "]";
    }
    return data;
}

QList<u16> value2IntegerOmronData( long int value, int length)
{
    QList<u16> data;
    for( int i=0; i<length; i++) {
        u16 number;
        number = value & 0xFFFFL;
        value = value >> 16;
        data.append( number);
    }
    return data;
}

void PlcProxy::writeValueF(QString addr, EplcDataType dataType, double value)
{
    QList<u16> data;
    switch( dataType) {
    case TYPE_FLOAT_2W:  data = float2omronData( value, 2);break;
    case TYPE_FLOAT_4W:  data = float2omronData( value, 4);break;
    default:
        qDebug() << "Unhandled input type value -> plc data writeValueF";
    }
    if( data.length() == 0) {
        qDebug() << "Conversion fail for write to " << addr;
        return;
    }
    qDebug() << "Write DBL to PLC " << QString("%1 @ %2/%3").arg(value).arg(addr).arg(dataType);
    writeData( addr, data);
}

void PlcProxy::writeValueI(QString addr, EplcDataType dataType, long int value)
{
    QList<u16> data;
    switch( dataType) {
    case TYPE_BCD:       data = value2BcdOmronData( value, 1); break;
    case TYPE_BCD_2W:    data = value2BcdOmronData( value, 2); break;
    case TYPE_BCD_4W:    data = value2BcdOmronData( value, 4); break;
    case TYPE_SWORD:
    case TYPE_UWORD:     data = value2IntegerOmronData( value,1); break;
    case TYPE_SWORD_2W:
    case TYPE_UWORD_2W:  data = value2IntegerOmronData( value,2); break;
    case TYPE_SWORD_4W:
    case TYPE_UWORD_4W:  data = value2IntegerOmronData( value,4); break;

    default:
        qDebug() << "Unhandled input type value -> plc data writeValueI";
    }
    if( data.length() == 0) {
        qDebug() << "Conversion fail for write to " << addr;
        return;
    }
    qDebug() << "Write INT to PLC " << QString("%1 @ %2/%3").arg(value).arg(addr).arg(dataType);
    writeData( addr, data);
}

/** write string to plc memory - this is C null terminated string
  */
void PlcProxy::writeValueString(QString addr, QString value)
{
    QList<u16> data;
    int i;
    u16 v;

    for( i = 0; i<value.length(); i++ ) {
        if( (i % 2) == 0) {
            v = (value.at( i).toLatin1()) << 8;
            data.append( v);
        };
        if( (i % 2) == 1) {
            v = value.at( i).toLatin1();
            data.last() = data.last() | v;
        }
    }
    v=0; //append null terminator
    data.append( v);
    writeData( addr, data);
}

/** read addres, emit signal
  */
void PlcProxy::readData( QString addr)
{
    qCritical() << "Not implemented PlcProxy::readData";
}

/**
  * Start Thread main loop
  */
void PlcProxy::startProxy(bool forceNotify)
{
    qDebug() << "tPlcProxy::startProxy()" << this;
    if( !isRunning()) {
        quit = false;
        start();
    }
    this->forceNotify = forceNotify;
}

/**
  * Stop Thread main loop - and wait for stop
  */
void PlcProxy::stopProxy()
{
    qDebug() << "tPlcProxy::stopProxy()" << this;
    if( isRunning()) {
        quit = true;
        wait();
    }
}
/*
void PlcProxy::performReadTests( void)
{
    //performance check for array read
    QVector<u16> datav10(10);
    QVector<u16> datav100(100);
    QVector<u16> datav200(200);

    plc->readData("C0", datav10);
    plc->readData("C0", datav100);
    plc->readData("C0", datav200);

    //test byte write to one addres
    for( int i=1; i<65536; i*=2) {
        u16 readData;
        plc->writeData("C10",i);
        plc->readData("C10",readData);
        QString res;
        for( int j=0; j<16; j++) {
            u16 tmpData;
            QString testAdr;
            testAdr = QString("C10:%1").arg(j);
            plc->readData( testAdr, tmpData);
            res += QString("%1").arg(tmpData);
        }
        qDebug() << i << QString(" %1 ").arg(readData) << res;
    }
    //
    //test bit write to one addres -ones
    for( int i=0; i<16; i++) {
        u16 readData;
        plc->writeData( QString("C100:%1").arg(i),1);
        plc->readData("C100",readData);
        QString res;
        for( int j=0; j<16; j++) {
            u16 tmpData;
            QString testAdr;
            testAdr = QString("C100:%1").arg(j);
            plc->readData( testAdr, tmpData);
            res += QString("%1").arg(tmpData);
        }
        qDebug() << i << QString(" %1 ").arg(readData) << res;
    }
    //test bit write to one addres - zeros
    for( int i=0; i<16; i++) {
        u16 readData;
        plc->writeData( QString("C100:%1").arg(i),0);
        plc->readData("C100",readData);
        QString res;
        for( int j=0; j<16; j++) {
            u16 tmpData;
            QString testAdr;
            testAdr = QString("C100:%1").arg(j);
            plc->readData( testAdr, tmpData);
            res += QString("%1").arg(tmpData);
        }
        qDebug() << i << QString(" %1 ").arg(readData) << res;
    }
}*/

/** Convert string addres to linear space plus bitmask if addres is for bit
  * Compress PLC memory to linear space
  * \return zero if addres if invalid
  */
unsigned int PlcProxy::addrToPhy( const QString& addr, u16 *bitmask)
{
    QByteArray finsAddr;
    finsAddr = adrToFinsAddr( addr);

    //fins addres struct: [desctriptor][addrMSB][addrLSB][biteNo]

    unsigned int base=0; //linear base for descriptor
    unsigned char mt = finsAddr[0];
    bool bitWide = false;
    switch( mt) {
            case 0x30:  bitWide=true; //C
            case 0xB0:  base = 0x10000;
                        break;

            case 0x31:  bitWide=true;//W
            case 0xB1:  base = 0x20000;
                        break;

            case 0x32:  bitWide=true;//H
            case 0xB2:  base = 0x30000;
                        break;

            case 0x33:  bitWide=true;//A
            case 0xB3:  base = 0x40000;
                        break;

            case 0x02:  bitWide=true;//D
            case 0x82:  base = 0x50000;
                        break;
            default:
                        //oops
                        qWarning() << QString("Bad input descriptor in tPlcProxy::addrToPhy, addr:%1").arg(addr);
                        return 0;
    };

    unsigned int saddr = ((unsigned char)finsAddr[1]) * 256 + ((unsigned char)finsAddr[2]);

    if( bitmask) {
        //convert bitaddr to bitmask
        u16 bm;
        if( bitWide) {
            bm = 1 << finsAddr[3];
        } else {
            bm = 0;
        }

        *bitmask = bm;
    }
    //qDebug() << "                        plxproxy " << addr << " " << base+saddr << " " << saddr;
    return base + saddr;

}

/**
  * Register addres for send notify updates
  */
void PlcProxy::itemRegister( QString addr, unsigned int itemSize, bool forceNotify)
{
    int vaddr; //virtual addres
    u16 bitmask;
    vaddr = addrToPhy( addr, &bitmask);

    if(!vaddr) {
        //bad conversion - abort
        qWarning() << QString("Aborted add to itemRegister, addr %1").arg(addr);
        return;
    }

    //fill str
    sRegItem item;
    item.vaddress    = vaddr;
    item.plcAdr      = addr;
    item.bitmask     = bitmask;
    item.forceNotify = forceNotify;
    item.itemSize    = itemSize;

    //add item to list of registered items
    regItems.insertMulti( vaddr, item); //allow duplicate items
}

int PlcProxy::readProxyRegion( QString addr, int length, bool gForceNotify)
{
    if( !plc) return -1;

    if( length > MAX_ENABLED_READ_LEN) {
        //! \todo Perform segmentation read here
        qWarning() << QString("Uniplemented segmentation read in readProxyRegion") << addr;
        return -2;
    }

    //read data from PLC
    QVector<u16> data(length);
    int s;

    s = plc->readData( addr, data);
    if( s) {
        //! \todo perform some action here
        qWarning() << QString("Plc read fail in readProxyRegion") << addr;
        return -2;
    }

    //convert begin addres to proxy vitrual addres
    unsigned int vaddr; //virtual addr - inted to proxy memory
    vaddr = addrToPhy( addr);
    if( !vaddr) {
        qWarning() << QString("Plc conversion to proxy memory address fail in readProxyRegion") << addr;
        return -3;
    }

    //sanity checks
    if( data.size() != length) {
        qWarning() << QString("Read with bad length readProxyRegion") << addr;
        return -4;
    }

    //write to memory and notify registered listener
    for( int i=0; i < data.size(); i++) {
        int va = i + vaddr;
        //check for registeredlisteners - multiples per one
        int items = regItems.count( va);
        if( items) {
            //some listeners registered

            u16 plcValue = data[i];
            u16 proxyValue = proxyMemory[va];

            // fetch listeners regs - all must be fetched for check forcedNotify
            QList<sRegItem> listeners;
            listeners = regItems.values( va);
            //iterate via listenerers
            for( int j=0; j<listeners.length(); j++) {
                u16 valueToNotify = plcValue;   //value to emmit (changed if bit wise value)
                //check value change
                //for multiwordchange items only one signal is emmited,
                //because registration is done only to one(first) PLC addres
                bool wordChanged, multiwordchange;
                wordChanged = (plcValue != proxyValue);
                multiwordchange = false;
                for( unsigned int k=0; k<listeners[j].itemSize; k++) {//check for multiword items
                    if( va+k > PROXY_MEMORY_SIZE || i>=data.size()) {
                        qWarning() << "Bad size for item " << listeners[j].plcAdr;
                        break;
                    }
                    if( data[i+k] != proxyMemory[va+k]) {
                        multiwordchange = true;
                        break;
                    }
                }

                //check if this is addr is bit wide, if yes check if coresponding bit changed
                u16 bitmask = listeners[j].bitmask;
                bool isBitWide;
                bool bitChange;
                if( bitmask) {
                    //it is nit addres, check for change
                    isBitWide = true;
                    bitChange = (( proxyValue & bitmask) != (plcValue & bitmask));
                    valueToNotify = ((plcValue & bitmask) != 0);    //change notify value to bool
                } else {
                    //no bitchange
                    isBitWide = false;
                    bitChange = false;
                }

                //check if forced or some data change
                //if any emit signal
                if(     gForceNotify                        //global force notify
                        || listeners[j].forceNotify         //forced notify
                        || bitChange                        //bit changed
                        || ((!isBitWide) && wordChanged)    //word changed and listener is byte wide
                        || multiwordchange
                ) {
                    //some change

                    /* if( !multiwordchange) {
                    //single element
                    qDebug() << QString("readProxyRegion Emit %1,%2,[%3,%4] value")
                                .arg(listeners[j].plcAdr,5)
                                .arg(valueToNotify,4,16)
                                .arg(plcValue,4,16)
                                .arg(proxyValue,4,16);
                    emit readed( listeners[j].plcAdr, valueToNotify);
                    } else {
                    Obsolete */

                    //rmulti word element
                    QList<u16> notifyData;
                    unsigned int m=0;
                    do {
                        notifyData.append( data[i+m]);
                        m++;
                    } while (  m<listeners[j].itemSize);

                    #ifdef PLC_PROXY_DEBUG
                    qDebug() << QString("readProxyRegion Emit %1,[%2] dataSize=%3, vaddr=%4")
                                .arg( listeners[j].plcAdr,5)
                                .arg( valueToNotify)
                                .arg( notifyData.size())
                                .arg( va);
                    #endif
                    if( isBitWide) {
                        //for bit wide date emit only 0 or 1, not word value of readed register
                        QList<u16> bitModData;
                        bitModData.append( valueToNotify);
                        emit readed( listeners[j].plcAdr, bitModData);
                    } else {
                        emit readed( listeners[j].plcAdr, notifyData);
                    }
                }
            }

        } else {
            //easy no listeners
            //no action
        }

        //after all overwrite old proxy value with new one
        proxyMemory_lock.lockForWrite();
        proxyMemory[ va] = data[i];
        proxyMemory_lock.unlock();
    } //next data cell
    return 0;
}

//! trhread main loop
void PlcProxy::run()
{
    qDebug() << "tPlcProxy::run()" << this;

    //while( quit) {
    //    msleep( 10);
    //};
    //return;
    //qDebug() << "tPlcProxy::run() --- end" << this;


    plcOpen();

    QTime checkTime;    //global timer to check regions
    checkTime.start();
    bool forceReadback = true;
    //Main loop - begin
    do {

        //write queued data if found
        if( !writeQueue.isEmpty() ) {
            writeQueue_lock.lockForWrite();
            while (!writeQueue.isEmpty()) {
                wq_type qe;
                qe = writeQueue.dequeue();
                if( writeEnabled) {
                    /* debug string*/
                    QString dta; dta.append("["); for(int di=0; di<qe.data.length(); di++) {dta.append(QString("%1 ").arg(qe.data[di]));};dta.append("]");
                    qDebug() << "Write to PLC from proxy " << dta << " @ " << qe.addr;
                    int s;
                    s=plc->writeData( qe.addr, qe.data);
                    if( s != 0) {
                        //runtiime comm error
                        emit error( ERR_WRITE_ERROR);
                        msleep(100);
                    }
                } else {
                    qDebug() << "--FAKE:writeDisable-- Write to PLC from proxy " << qe.data << " @ " << qe.addr;
                }
            }
            writeQueue_lock.unlock();
            forceReadback = true; //readback
        } else {
            msleep( 10);
        };

        if( checkTime.elapsed() > 50 || forceReadback) {

            QTime currTime = QTime::currentTime();  //current time
            //get values from PLC - iterate through regions
            for( int i=0; i<regions.length(); i++) {
                region_type region;

                regions_lock.lockForRead();
                region = regions[i];
                regions_lock.unlock();

                //check if set time elapsed for region
                int elapsed;
                //abs is here, it is for external time change events like DST, system time update to back,..
                elapsed = abs( region.lastScan.msecsTo( currTime));
                if( elapsed >= region.scanInterval || forceReadback) {
                    int s;
                    s=readProxyRegion( region.region, region.count, forceNotify);
                    if( s != 0) {
                        //runtiime comm error
                        emit error( ERR_READ_ERROR);
                        msleep(100);
                    } else {
                        emit cycleOk();
                    }

                    //update region last scan time
                    regions_lock.lockForWrite();
                    regions[i].lastScan = currTime;
                    regions_lock.unlock();

                }

            }

            forceReadback = false;
            checkTime.restart();
        }

    } while( !quit);

    qDebug() << QString("tPlcProxy::run() - normal exit - cleanup");

    plcClose();

}

/**
  * REad memory value cached from proxy
  * \todo there is no guaranty if value was readed from PLC
  */
u16 PlcProxy::readProxyData( QString addr, bool *status)
{
    //convert addres to proxy vitrual addres
    unsigned int vaddr; //virtual addr - inted to proxy memory
    u16 bitmask;
    vaddr = addrToPhy( addr, &bitmask);
    if( !vaddr) {
        qWarning() << QString("Plc conversion to proxy memory address fail in readProxyData") << addr;

        if( status) *status = false;
        return 0;
    }

    //sanity check
    if( vaddr > PROXY_MEMORY_SIZE) {
        qWarning() << QString("Out of boundary addr fail in readProxyData") << addr;

        if( status) *status = false;
        return 0;
    }

    //obtain proxy value
    u16 value;
    proxyMemory_lock.lockForRead();
    value = proxyMemory[vaddr];
    proxyMemory_lock.unlock();

    if( status) *status = true;
    if( bitmask) {
        return value&bitmask ? 1:0;
    } else {
        return value;
    }
}

/**
  * REad memory value cached from proxy
  * \todo there is no guaranty if value was readed from PLC
  */
QList<u16> PlcProxy::readProxyDatas( QString addr, int length,  bool *status)
{
    //convert addres to proxy vitrual addres
    unsigned int vaddr; //virtual addr - inted to proxy memory
    u16 bitmask;
    QList<u16> r;
    vaddr = addrToPhy( addr, &bitmask);
    if( !vaddr) {
        qWarning() << QString("Plc conversion to proxy memory address fail in readProxyData") << addr;

        if( status) *status = false;
        return r;
    }
    if( bitmask) {
        qWarning() << QString("Bit address not alowed in multiple data read") << addr;

        if( status) *status = false;
        return r;
    }

    //sanity check
    if( vaddr > PROXY_MEMORY_SIZE) {
        qWarning() << QString("Out of boundary addr fail in readProxyData") << addr;

        if( status) *status = false;
        return r;
    }

    //obtain proxy value
    proxyMemory_lock.lockForRead();
    for( int i=0;i<length;i++) {
        r << proxyMemory[vaddr+i];
    }
    proxyMemory_lock.unlock();

    if( status) *status = true;
    return r;
}

void PlcProxy::flushRegions( void)
{
    regions_lock.lockForWrite();
    regions.clear();
    regions_lock.unlock();
}

int PlcProxy::addRegion(QString beginAddr, int length, int scanInterval)
{
    if( !addrToPhy( beginAddr)) {
        qWarning() << "Bad region addr setup for plcProxy " << beginAddr;
        return -1;
    }
    if( length<=0 || length >MAX_ENABLED_READ_LEN) {
        qWarning() << "Bad region length setup for plcProxy " << length;
        return -2;
    }
    //setup new region
    region_type newReg;
    newReg.region = beginAddr;
    newReg.count = length;
    newReg.lastScan = QTime::currentTime();
    newReg.lastScan.addMSecs( -(scanInterval+100)); //some setup to force scan
    newReg.scanInterval = scanInterval;

    //add new region to list
    regions_lock.lockForWrite();
    regions.append( newReg);
    regions_lock.unlock();

    return 0;
}
