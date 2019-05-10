#include "widgetplcinterface.h"
#include <QDebug>

WidgetPlcInterface::WidgetPlcInterface()
{
}

void WidgetPlcInterface::updatePlcValue( QList<u16> data)
{
    if(data.length() <= 0) {
        qWarning() << "empty call to tWidgetPlcInterface::updatePlcValue( QList<u16> data)";
        return;
    }
    if( data[0]) {
        emit ValueIsTrue();
    } else {
        emit ValueIsFalse();
    }
    emit NewValue();
}
