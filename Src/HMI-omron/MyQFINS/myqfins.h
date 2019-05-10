#ifndef MYQFINS_H
#define MYQFINS_H

#include <QObject>
#include <QTimer>
#include "plcqlib.h"

class MyQFINS : public QObject
{
    Q_OBJECT
public:
    explicit MyQFINS(QString IP, QObject *parent = nullptr);
    ~MyQFINS();


    void startPlc1Comm();
    void initPlc1WidgetsStructure();
public slots:
    void writeData(QString addr, u16 value);
    void readed(QString addr, QList<u16> data);
    void timeout();
signals:
    void updateVAR(QString addr, QList<u16> data);

private slots:
    void plc1ProxyError(PlcProxy::EplcProxyError error);
    void plc1Restart();



private:
    //PLC
    PlcProxy                *plc1Proxy;
    PlcEventHandler         plc1EventHandler;    //proxy to widgets
    PlcWidgetEventHandler   widget1EventHandler; //widgets to proxy

    QTimer *plc1RestartTimer;
    QTimer *timer1;
};

#endif // MYQFINS_H
