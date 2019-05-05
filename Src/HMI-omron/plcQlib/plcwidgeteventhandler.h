#ifndef PLCWIDGETEVENTHANDLER_H
#define PLCWIDGETEVENTHANDLER_H

#include <QObject>
#include "plcproxy.h"
#include "plcspinboxsender.h"

class PlcWidgetEventHandler : public QObject
{
Q_OBJECT
public:
    explicit PlcWidgetEventHandler(QObject *parent = 0);

    struct winfo_type {
        QString                 addr;
        WidgetPlcSender         *sendingWidget;
        PlcProxy::EplcDataType  plcVariableType;
    };

    typedef QHash<
       QObject*, winfo_type>  wdgtEvtList_type;
    typedef QHashIterator<
       QObject*, winfo_type>  wdgtEvtList_iter;

public:
    ~PlcWidgetEventHandler();
    void Init( PlcProxy* plcProxy);

    //! \brief inserted objects are owned by source widget object !
    void Add( QString addr, WidgetPlcSender *sendingWidget,  PlcProxy::EplcDataType plcVariableType);

private:
    PlcProxy *plcProxy;

    //! list of plcActions registered for update from PLC read values
    wdgtEvtList_type  wdgtEventsList;

signals:

public slots:
    void sendValueToPlc_double( double value);
    void sendValueToPlc_int( long int value);
    void sendValueToPlc_str( QString value);

};

#endif // PLCWIDGETEVENTHANDLER_H
