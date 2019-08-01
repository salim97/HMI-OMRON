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
    ZONE_MEMOIRE("26.00", h200)




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


