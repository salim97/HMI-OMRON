#ifndef MYOMRON_H
#define MYOMRON_H

#include <QObject>
#include <QTimer>
#include "plcqlib.h"
#include "mypropertyhelper.h"

class MyOMRON : public QObject
{
    Q_OBJECT
public:
    explicit MyOMRON(QString ip, QObject *parent = nullptr);
    ~MyOMRON();

    AUTO_PROPERTY(bool, isConnected)

    ZONE_MEMOIRE("D1", d1)
    ZONE_MEMOIRE("H2.00", h200)




    void startPlc1Comm();
signals:

public slots:

private slots:
    void plc1ProxyError(PlcProxy::EplcProxyError error);
    void plc1Restart();
    void _syncTimerTimeout();
private:
    PlcProxy  *plc1Proxy;
    QTimer *plc1RestartTimer;
};

#endif // MYOMRON_H


