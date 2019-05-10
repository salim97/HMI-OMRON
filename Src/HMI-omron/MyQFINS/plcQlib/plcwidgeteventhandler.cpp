#include "plcwidgeteventhandler.h"

PlcWidgetEventHandler::PlcWidgetEventHandler(QObject *parent) :
    QObject(parent)
{
    plcProxy=NULL;
}

/**
  * no destroy to owned events - owned by widgets
  */
PlcWidgetEventHandler::~PlcWidgetEventHandler()
{
    /*
    wdgtEvtList_iter    i(wdgtEventsList);
    while( i.hasNext()) {
        i.next();
        if( i.value() ) {
            //if exists, delete
            qDebug() << "PlcWidgetEventHandler delete " << i.key();// << i.value()->getTargetName();
            delete i.value();
        }
    }
    */
    qDebug() << "PlcWidgetEventHandler - normal cleanup";
}

void PlcWidgetEventHandler::Init( PlcProxy* plcProxy)
{
    this->plcProxy = plcProxy;
}

//Add new item
void PlcWidgetEventHandler::Add(QString addr, WidgetPlcSender *sendingWidget,  PlcProxy::EplcDataType plcVariableType)
{
    //insert to list
    winfo_type wi;
    wi.plcVariableType = plcVariableType;
    wi.sendingWidget   = sendingWidget;
    wi.addr            = addr;

    wdgtEventsList.insertMulti( sendingWidget, wi);

    //register signals
    switch( plcVariableType) {
    case PlcProxy::TYPE_UWORD:
    case PlcProxy::TYPE_UWORD_2W:
    case PlcProxy::TYPE_UWORD_4W:

    case PlcProxy::TYPE_SWORD:
    case PlcProxy::TYPE_SWORD_2W:
    case PlcProxy::TYPE_SWORD_4W:

    case PlcProxy::TYPE_BCD:
    case PlcProxy::TYPE_BCD_2W:
    case PlcProxy::TYPE_BCD_4W:
        QObject::connect( sendingWidget, SIGNAL(valueChanged_int(long int)),this,SLOT(sendValueToPlc_int(long int)));
        break;

    case PlcProxy::TYPE_FLOAT_2W:
    case PlcProxy::TYPE_FLOAT_4W:
        QObject::connect( sendingWidget, SIGNAL(valueChanged_double(double)),this,SLOT(sendValueToPlc_double(double)));
        break;

    case PlcProxy::TYPE_VOID:
    default:
        {};
    }
}

void PlcWidgetEventHandler::sendValueToPlc_double(double value)
{
    //get matched record to registered widgets details
    QList< winfo_type> matchedSenders;
    matchedSenders = wdgtEventsList.values( sender());

    //process requests
    winfo_type wi;
    for(int i=0; i<matchedSenders.length(); i++) {
        wi = matchedSenders[i];
        qDebug() << "Write to PLC "
                 << sender()->objectName() << " "
                 << wi.addr << " = "
                 << value << " / "
                 << wi.plcVariableType;
        plcProxy->writeValueF( wi.addr, wi.plcVariableType, value);
    }

    qDebug() << "sendValueToPlc_double " << value << " @ " << wi.addr;
}

void PlcWidgetEventHandler::sendValueToPlc_int(long int value)
{
    //get matched record to registered widgets details
    QList< winfo_type> matchedSenders;
    matchedSenders = wdgtEventsList.values( sender());

    //process requests
    winfo_type wi;
    for(int i=0; i<matchedSenders.length(); i++) {
        wi = matchedSenders[i];
        qDebug() << "Write to PLC "
                 << sender()->objectName() << " "
                 << wi.addr << " = "
                 << value << " / "
                 << wi.plcVariableType;
        plcProxy->writeValueI( wi.addr, wi.plcVariableType, value);
    }

    qDebug() << "sendValueToPlc_int " << value << " @ " << wi.addr;
}

void PlcWidgetEventHandler::sendValueToPlc_str(QString value)
{
    qDebug() << "sendValueToPlc_str " << value << " @ " ;
}
