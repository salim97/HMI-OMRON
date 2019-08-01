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
    ZONE_MEMOIRE("C200.00", c20000)
    ZONE_MEMOIRE("C201.00", c20100)
    ZONE_MEMOIRE("C202.00", c20200)
    ZONE_MEMOIRE("C203.00", c20300)
    ZONE_MEMOIRE("C204.00", c20400)
    ZONE_MEMOIRE("C205.00", c20500)
    ZONE_MEMOIRE("C206.00", c20600)





    void startPlc1Comm();
signals:

public slots:
    void readALL();
    void sendALL();
private slots:
    void plc1ProxyError(PlcProxy::EplcProxyError error);
    void plc1Restart();

private:
    PlcProxy  *plc1Proxy;
    QTimer *plc1RestartTimer;
};

#endif // MYOMRON_H


