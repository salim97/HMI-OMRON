#ifndef PLCWIDGETSENDER_H
#define PLCWIDGETSENDER_H

#include <QObject>
#include <QAbstractButton>
#include <QString>


#include "plcproxy.h"
#include "types.h"


class PlcWidgetSender : public QObject
{

Q_OBJECT

public:
    PlcWidgetSender( QString addr, QAbstractButton *button, PlcProxy *plcProxy);

    //setup functions
    PlcWidgetSender* ZeroIfClicked();
    PlcWidgetSender* OneIfClicked();
    PlcWidgetSender* OneWhenPressed();
    PlcWidgetSender* PulseOneIfClicked();
    PlcWidgetSender* ValueIfTogled();
    PlcWidgetSender* IOPressButton(bool inverted=false);
    PlcWidgetSender* PressToInvert();

signals:
    void PlcWriteData( QString addr, u16 value);
    void sendValue( bool);  //send data to widget

public slots:
    void SendOne(void);
    void SendZero(void);
    void SendChecked( bool);
    void SendTruePulse();
    void SendInvert();

    void dataFromPlc( QString addr, QList<u16> data); //slot for read data from PLC

private:
    QString         addr;   //!< destination addr in plc
    QAbstractButton *button;
    bool            inverted;
    PlcProxy        *plcProxy;

};

#endif // PLCWIDGETSENDER_H
