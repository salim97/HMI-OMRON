#include "myomron.h"
#include "math.h"

MyOMRON::MyOMRON(QString ip, QObject *parent) : QObject(parent)
{

    plc1Proxy = new PlcProxy(ip);

    //register for read errors from proxy
    QObject::connect( plc1Proxy, SIGNAL(error(PlcProxy::EplcProxyError)), this, SLOT(plc1ProxyError(PlcProxy::EplcProxyError)));
    //initPlc1WidgetsStructure(); //setup notifiers
    startPlc1Comm();
    QTimer::singleShot(1000, [this]() { qDebug() << "QTIMER CA MARCHE " ; readALL(); } );


    //restart timer when comm fail event
    plc1RestartTimer = new QTimer(this);
    plc1RestartTimer->setInterval( 3000);
    QObject::connect( plc1RestartTimer, SIGNAL(timeout()), this, SLOT(plc1Restart()));

    //connect(plc1Proxy, SIGNAL(readed(QString,QList<u16>)), this, SLOT(readed(QString,QList<u16>)));



    //CALL_THIS_IN_CONSTRACTEUR_FOR_ZONE_MEMOIRE_AUTO_UPDATE
//    QTimer *_syncTimer ;
//    _syncTimer = new QTimer(this);
//    _syncTimer->setInterval(100);
//    _syncTimer->setSingleShot(false);
//    connect(this, SIGNAL(destroyed(QObject*)), _syncTimer, SLOT(deleteLater()));

//    connect(_syncTimer, SIGNAL(timeout()), this, SLOT(_syncTimerTimeout()));
//    _syncTimer->start();
}

MyOMRON::~MyOMRON()
{
    plc1Proxy->stopProxy();
    delete plc1Proxy; plc1Proxy = nullptr;
}

void MyOMRON::bit()
{
    pow(2,2);
}

void MyOMRON::readALL()
{
    //qDebug() << Q_FUNC_INFO << "timeout " ;
    for(int i = this->metaObject()->methodOffset();
        i < this->metaObject()->methodCount(); i++) {
        if(this->metaObject()->method(i).name().contains("_read"))
            this->metaObject()->invokeMethod(this,
                                             this->metaObject()->method(i).name(),
                                             Qt::DirectConnection);
    }
    isConnected(plc1Proxy->plcIsOk());

}


void MyOMRON::sendALL()
{
    for(int i = this->metaObject()->methodOffset();
        i < this->metaObject()->methodCount(); i++) {
        if(this->metaObject()->method(i).name().contains("_send"))
            this->metaObject()->invokeMethod(this,
                                             this->metaObject()->method(i).name(),
                                             Qt::DirectConnection);
    }
}


void MyOMRON::startPlc1Comm()
{
    if( plc1Proxy) {

        plc1Proxy->flushRegions(); //delete regions registrations
        //plcProxy->disableWrite();

        //Main PLC
        // TODO : hadi pour sacni un region avec interval du temp
        //plc1Proxy->addRegion("H0",100,200);
        //plc1Proxy->addRegion("H100",100,200);

       // plc1Proxy->addRegion("D0", 3 ,500);
        plc1Proxy->addRegion("D0", 100 ,500);

        //read all values from PLC
        plc1Proxy->startProxy( true); //force notify - init whole gui
        plc1Proxy->stopProxy();
        plc1Proxy->startProxy();
        plc1Proxy->enableWrite();


    } else {
        qCritical() << "PLC proxy object not found ..";
    }
}

void MyOMRON::plc1ProxyError(PlcProxy::EplcProxyError error)
{
    qDebug() << "PLC ERROR" << error;

    if( !plc1RestartTimer->isActive()) {
        qDebug() << " ...Start Timer";
        plc1RestartTimer->start( 3000);
    }
}

void MyOMRON::plc1Restart()
{
    qDebug() << "PLC RESTART";
    plc1RestartTimer->stop();
    plc1Proxy->stopProxy();
    startPlc1Comm();


}

