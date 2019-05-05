#ifndef TWIDGETPLCINTERFACE_H
#define TWIDGETPLCINTERFACE_H

#include <QList>
#include <QObject>
#include "types.h"

/**
  * Object for interface between plcProxy and listener,
  * emit signals, when value is true, false, new value come...
  */
class WidgetPlcInterface
    :public QObject
{
    Q_OBJECT

public:
    WidgetPlcInterface();
    virtual ~WidgetPlcInterface() {};
//    virtual void updatePlcValue( u16){};

public slots:

    void updatePlcValue( QList<u16>);
//    virtual QString plcObjectName() = 0;

signals:
    void ValueIsTrue();
    void ValueIsFalse();
    void NewValue();
};

#endif // TWIDGETPLCINTERFACE_H
