#ifndef TPLCPROXY_H
#define TPLCPROXY_H

#include <QThread>
#include <QQueue>
#include <QTime>
#include <QReadWriteLock>

#include "omronfinscom.h"

#define PROXY_MEMORY_SIZE 0x60000   //!< depend on implementation in addrToPhy !!


//canversion function (in plcevent.cpp)
double omronData2double( QList<u16> data);
unsigned int omronData2uint( QList<u16> data);
int omronData2int( QList<u16> data);
unsigned int omronBcdData2uint( QList<u16> data);
//proxy dtata prepare:
QList<u16> value2IntegerOmronData( long int value, int length);
QList<u16> value2BcdOmronData( long int value, int length);
QList<u16> float2omronData( double value, int length);


struct sRegItem {
    unsigned int    vaddress;   //!< virtual addres - map PLC D,W,C,A,... memories to linead region
    u16             bitmask;    //!< bitmask, nonzero for bit - with appropriate word to bit mask
    QString         plcAdr;     //!< origin registered PLC addres 'W100:1'
    unsigned int    itemSize;   //!< lengt of data , 0=no array(bit,word), >0 array lengts constraints
    bool            forceNotify; //!< allways send signal after every PLC read even with no value change
    sRegItem() {vaddress=0; bitmask=0x0; plcAdr=QString();itemSize=0;forceNotify=false;}
};

class PlcProxy : public QThread
{
    Q_OBJECT

public:
    PlcProxy( QString plcIPaddr, QObject *parent = 0);
    ~PlcProxy();

    enum EplcDataType {
        TYPE_VOID,

        TYPE_UWORD,
        TYPE_SWORD,
        TYPE_BCD,

        TYPE_UWORD_2W,
        TYPE_SWORD_2W,
        TYPE_BCD_2W,
        TYPE_FLOAT_2W,

        TYPE_UWORD_4W,
        TYPE_SWORD_4W,
        TYPE_BCD_4W,
        TYPE_FLOAT_4W
    };

    enum EplcProxyError {
        ERR_NO_ERROR        =   0x000000,
        ERR_READ_ERROR      =   0x000100,
        ERR_WRITE_ERROR     =   0x000200
    };

    bool testConnection();

    bool plcIsOk();

    int  addRegion( QString beginAddr, int length, int scanInterval=5000);
    void flushRegions();

    void itemRegister( QString addr, unsigned int itemSize=0, bool forceNotify=false);   //!< register addres for send update
    //void unregister - TODO

    //void writeValueS( QString addr, EplcDataType dataType, signed long value );
    //void writeValueU( QString addr, EplcDataType dataType, unsigned long value );
    void writeValueF( QString addr, EplcDataType dataType, double value );
    void writeValueI( QString addr, EplcDataType dataType, long int value);
    void writeValueString( QString addr, QString value);

    void startProxy(bool forceNotify=false);
    void stopProxy();
    void enableWrite()  {writeEnabled=true;}
    void disableWrite() {writeEnabled=false;}

    u16 readProxyData( QString addr, bool *status=0); //!< \brief Read proxy value    
    QList<u16> readProxyDatas(QString addr, int length=1, bool *status=0);

public slots:
    void writeData( QString addr, u16 value) {
        QList<u16> data;
        data.append( value);
        writeData(addr,data);
    }
    void writeData( QString addr, QList<u16> data);
    void readData(  QString addr);

signals:
    // obsolette void readed( QString addr, u16 value);
    void readed( QString addr, QList<u16> data);
    void error( PlcProxy::EplcProxyError errorCode);
    void cycleOk();

private:
    OmronPlc *plc;
    bool fakePLC; //!< simulate PLC, no call to plc soubroutines. This is another mode as plc simulation!

    int  plcOpen();
    void plcClose();

    bool forceNotify;
    bool writeEnabled;

    struct region_type {
        QString     region;     //begin addr
        int         count;      //number of items
        int         scanInterval; //scanning interval in msec
        QTime       lastScan;   //time for next scan
    };

    QList<region_type> regions;
    QReadWriteLock regions_lock;

    QString plcIPaddr;

    //Thread related
    bool quit;

    void run(); //main loop


    //QWaitCondition cond;

    //Proxy related
    QHash<int, sRegItem> regItems;  //!< registered address for read notification

    u16  proxyMemory[ PROXY_MEMORY_SIZE+64];    //!< PLC linear proxy memory
    QReadWriteLock proxyMemory_lock;

    //write queue
    struct wq_type { QString addr; QList<u16> data; };
    QQueue<wq_type> writeQueue;
    QReadWriteLock  writeQueue_lock;

    unsigned int addrToPhy( const QString& addr, u16 *bitmask=0); //!< \brief convert string addres to virtual (linear) addres
    int readProxyRegion( const QString addr, int length, bool gForceNotify=false);         //!< \brief read region into proxy memory

    //debug and unit tests
    //void performReadTests( void); //simple test
};

Q_DECLARE_METATYPE(PlcProxy::EplcProxyError)
#endif // TPLCPROXY_H
