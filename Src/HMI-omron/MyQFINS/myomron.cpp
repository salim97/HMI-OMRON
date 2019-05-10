#include "myomron.h"

MyOMRON::MyOMRON(QString ip, QObject *parent) : QObject(parent)
{
    myQFINS = new MyQFINS(ip, this);

}
