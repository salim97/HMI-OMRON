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


public slots:

signals:

private slots:



private:
};

#endif // MYQFINS_H
