#include "myomron.h"

MyOMRON::MyOMRON(QString ip, QObject *parent) : QObject(parent)
{
    myQFINS = new MyQFINS(ip, this);
    connect(myQFINS, SIGNAL(updateVAR(QString,QList<u16>)), this, SLOT(readed(QString,QList<u16>)));
}

void MyOMRON::readed( QString addr, QList<u16> data)
{
    qDebug() << Q_FUNC_INFO << addr << data ;
    if( addr == "D1")
        d1(data.first());
}
