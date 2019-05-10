#include "myqfins.h"

MyQFINS::MyQFINS(QString IP, QObject *parent) : QObject(parent)
{
    plc1Proxy = new PlcProxy(IP);

    //register for read errors from proxy
    QObject::connect( plc1Proxy, SIGNAL(error(PlcProxy::EplcProxyError)), this, SLOT(plc1ProxyError(PlcProxy::EplcProxyError)));

    //initPlc1WidgetsStructure(); //setup notifiers
    startPlc1Comm();

    //restart timer when comm fail event
    plc1RestartTimer = new QTimer(this);
    plc1RestartTimer->setInterval( 3000);
    QObject::connect( plc1RestartTimer, SIGNAL(timeout()), this, SLOT(plc1Restart()));

    connect(plc1Proxy, SIGNAL(readed(QString,QList<u16>)), this, SLOT(readed(QString,QList<u16>)));
}

MyQFINS::~MyQFINS()
{
    plc1Proxy->stopProxy();
    delete plc1Proxy; plc1Proxy = nullptr;
}

void MyQFINS::writeData( QString addr, u16 value)
{
    plc1Proxy->writeData(addr, value);
}

void MyQFINS::readed( QString addr, QList<u16> data)
{
    emit updateVAR(addr, data);
}




void MyQFINS::startPlc1Comm()
{
    if( plc1Proxy) {

        plc1Proxy->flushRegions(); //delete regions registrations
        //plcProxy->disableWrite();

        //Main PLC
        // TODO : hadi pour sacni un region avec interval du temp
        //plc1Proxy->addRegion("H0",100,200);
        //plc1Proxy->addRegion("H100",100,200);
        plc1Proxy->addRegion("D10000",100,1000);

        //read all values from PLC
        plc1Proxy->startProxy( true); //force notify - init whole gui
        plc1Proxy->stopProxy();
        plc1Proxy->startProxy();
        plc1Proxy->enableWrite();

    } else {
        qCritical() << "PLC proxy object not found ..";
    }
}
/*
void MyQFINS::initPlc1WidgetsStructure()
{
    qDebug() << "MainWindow::initPlcWidgetsStructure";
    plc1EventHandler.Init( plc1Proxy);    //inputs

    widget1EventHandler.Init( plc1Proxy); //outputs

    PlcWidgetSender *pws;


    //HMI - indikators - change vissibility from PLC:
    plc1EventHandler.Add( "H10:1", new PlcEvent_QWidget(PlcEvent::VISIBLE_IF_TRUE,  ui->l_h10_1_s1));

    //string formater for labels
    QTextStream *sf = new QTextStream( new QString());
    sf->setRealNumberPrecision( 1);
    sf->setRealNumberNotation( QTextStream::FixedNotation);

    QTextStream *sr = new QTextStream( new QString());
    sr->setRealNumberPrecision( 0);
    sr->setRealNumberNotation( QTextStream::FixedNotation);

    //HMI - indikators - numbers form PLC to labels:
    plc1EventHandler.Add( "H20", new PlcEvent_QLabel( PlcEvent::UPDATE_VALUE_FLOAT, ui->l_h20f_s3, sf, 2));
    plc1EventHandler.Add( "H19", new PlcEvent_QLabel( PlcEvent::UPDATE_VALUE_UNSIG, ui->l_h19w_s2, sr, 1));
    plc1EventHandler.Add( "H44", new PlcEvent_QLabel( PlcEvent::UPDATE_VALUE_BCD, ui->l_h44w_sd, sr, 1));

    //HMI - controls - set to one if clicked
    pws = new PlcWidgetSender( "H105:2",  ui->pb_h16_3_sd,       plc1Proxy); pws->OneIfClicked();
    pws = new PlcWidgetSender( "H109:0",  ui->pb_h109_0_off_s4,  plc1Proxy); pws->ZeroIfClicked();

    //HMI - controls - spinbox
    widget1EventHandler.Add( "H40", new SpinboxPlcSender(
               ui->sb_h40_sa, trUtf8("L1_Sno"),ui->sb_h40_sa),
               PlcProxy::TYPE_UWORD );

    plc1EventHandler.Add( "D10002", new PlcEvent_QDoubleSpinBox(PlcEvent::UPDATE_VALUE_FLOAT, ui->sb_d10002f_sd, 2)); //readback from plc
    widget1EventHandler.Add( "D10002", new SpinboxPlcSender(
               ui->sb_d10002f_sd, trUtf8("L1_W"),ui->sb_d10002f_sd),
               PlcProxy::TYPE_FLOAT_2W );


    //Finnaly:
    //register readback signal from Proxy to this object
    QObject::connect( this->plc1Proxy, SIGNAL(readed(QString,QList<u16>)),
                      &plc1EventHandler,SLOT(plcEventService(QString,QList<u16>)));
}


*/
void MyQFINS::plc1ProxyError(PlcProxy::EplcProxyError error)
{
    qDebug() << "PLC ERROR" << error;

    if( !plc1RestartTimer->isActive()) {
        qDebug() << " ...Start Timer";
        plc1RestartTimer->start( 3000);
    }
}

void MyQFINS::plc1Restart()
{
    qDebug() << "PLC RESTART";
    plc1RestartTimer->stop();
    plc1Proxy->stopProxy();
    startPlc1Comm();
}

