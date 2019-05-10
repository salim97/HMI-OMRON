#ifndef MYOMRON_H
#define MYOMRON_H

#include <QObject>
#include <QTimer>
#include "myqfins.h"
#include "mypropertyhelper.h"

class MyOMRON : public QObject
{
    Q_OBJECT
public:
    explicit MyOMRON(QString ip, QObject *parent = nullptr);

    ZONE_MEMOIRE("D1", d1)
    ZONE_MEMOIRE("H2.00", h200)


signals:

public slots:

private:
    MyQFINS *myQFINS;
};

#endif // MYOMRON_H


