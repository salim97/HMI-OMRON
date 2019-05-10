#include "plcevhandler.h"

PlcEventHandler::PlcEventHandler() {
    plcProxy=NULL;
}

/**
  * destroy owned events
  */
PlcEventHandler::~PlcEventHandler()
{
    plcEvtList_iter i(plcEventsList);
    while( i.hasNext()) {
        i.next();
        if( i.value() ) {
            //if exists, delete
            //qDebug() << "PlcEventHandler delete " << i.key();// << i.value()->getTargetName();
            delete i.value();
        }
    }
    qDebug() << "PlcEventHandler - normal cleanup";
}

void PlcEventHandler::Init( PlcProxy* plcProxy)
{
    this->plcProxy = plcProxy;

}

//Add new item
void PlcEventHandler::Add(QString addr, PlcEvent *plcEvent, bool forceNotify)
{
    plcEventsList.insertMulti(addr, plcEvent);
    plcProxy->itemRegister( addr, plcEvent->GetItemLength(),forceNotify);
}


/*callback slot when plc proxy read addres
void PlcEventHandler::plcEventService( QString addr, u16 value)
{
    //get addres to widgets object, call actions
    QList<PlcEvent *> matchedEvents;
    matchedEvents = plcEventsList.values( addr);

    //send to all matched widgets
    for(int i=0; i<matchedEvents.length(); i++) {
        PlcEvent *pa = matchedEvents[i];
        pa->HandleData( value);
    }
} Obsolete */


//callback slot when plc proxy read addres
void PlcEventHandler::plcEventService( QString addr, QList<u16> data)
{
    //get addres to widgets object, call actions
    QList<PlcEvent *> matchedEvents;
    matchedEvents = plcEventsList.values( addr);

    //send to all matched widgets
    for(int i=0; i<matchedEvents.length(); i++) {
        PlcEvent *pa = matchedEvents[i];
        pa->HandleData( data);

    }
}
