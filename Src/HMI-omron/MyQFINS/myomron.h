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

    ZONE_MEMOIRE("D1", d1)     // hadi mat sauvgardich ...

    ZONE_MEMOIRE_SETTINGS("H0", h0) // hadi t sauvgarde l'etat in local storage

    // Valid Etiq          h0.06
    // valid Co etiq       h0.04
    // valid etiq co etiq  h0.07
    // valid rep etiq      h0.03
    // valid rep cran      h0.09
    // valid plaquer       h0.05

    ZONE_MEMOIRE("C16", c16)
    // marche 16.13

    ZONE_MEMOIRE("D100", d100) // départ etiq
    ZONE_MEMOIRE("D101", d101) // départ co etiq
    ZONE_MEMOIRE("D102", d102) // départ etiq co etiq
    ZONE_MEMOIRE("D103", d103) // arret etiq
    ZONE_MEMOIRE("D104", d104) // arret co etiq
    ZONE_MEMOIRE("D105", d105) // arret etiq co etiq

    ZONE_MEMOIRE("D2", d2)     // plaquage
    ZONE_MEMOIRE("D3", d3)     // secu echenillage
    ZONE_MEMOIRE("D4", d4)     // secu etiq co etiq
    ZONE_MEMOIRE("D5", d5)     // rotation
    ZONE_MEMOIRE("D6", d6)     // lissage
    ZONE_MEMOIRE("D7", d7)     // secu reperage






    void bit();

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


