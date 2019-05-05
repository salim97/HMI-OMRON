#ifndef CPLCEVHANDLER_H
#define CPLCEVHANDLER_H

#include <QObject>

#include "plcevent.h"
#include "plcproxy.h"




class PlcEventHandler : public QObject
{
    Q_OBJECT

    typedef QHash< QString, PlcEvent *> plcEvtList_type;
    typedef QHashIterator< QString, PlcEvent *> plcEvtList_iter;

public:
    PlcEventHandler();
    ~PlcEventHandler();
    void Init( PlcProxy* plcProxy);

    //! \brief inserted objects are owned by this object !
    void Add( QString addr, PlcEvent *plcEvent, bool forceNotify=false);

private:
    PlcProxy *plcProxy;

    //! list of plcActions registered for update from PLC read values
    plcEvtList_type  plcEventsList;

    //! list of widgets registered to send data back to plc

public slots:
    // Obsolete void plcEventService( QString addr, u16 value);
    void plcEventService( QString addr, QList<u16> data);


};

#endif // CPLCEVHANDLER_H
