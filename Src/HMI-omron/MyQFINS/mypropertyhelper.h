#ifndef MYPROPERTYHELPER_H
#define MYPROPERTYHELPER_H

#pragma once
#include <QObject>
#include <QString>
#include <QTimer>
#include <QMetaMethod>

#define AUTO_PROPERTY(TYPE, NAME) \
    Q_PROPERTY(TYPE NAME READ NAME WRITE NAME NOTIFY NAME ## Changed ) \
    public: \
    TYPE NAME() const { return m_ ## NAME ; } \
    void NAME(TYPE value) { \
    if (m_ ## NAME == value)  return; \
    m_ ## NAME = value; \
    emit NAME ## Changed(value); \
    } \
    Q_SIGNAL void NAME ## Changed(TYPE value);\
    private: \
    TYPE m_ ## NAME;

#define READONLY_PROPERTY(TYPE, NAME) \
    Q_PROPERTY(TYPE NAME READ NAME CONSTANT ) \
    public: \
    TYPE NAME() const { return m_ ## NAME ; } \
    private: \
    void NAME(TYPE value) {m_ ## NAME = value; } \
    TYPE m_ ## NAME;

#define ZONE_MEMOIRE(ADDRESS, NAME) \
    Q_PROPERTY(int NAME READ NAME WRITE NAME NOTIFY NAME ## Changed ) \
    public: \
    int NAME() const { return m_ ## NAME ## _value ; } \
    void NAME ## _update() {\
    qDebug() << Q_FUNC_INFO << sender()->metaObject()->className();  \
    NAME( plc1Proxy->readProxyData("D1") ) ;  \
    } \
    void NAME(int value) { \
    if (m_ ## NAME ## _value == value)  return; \
    m_ ## NAME ## _value = value; \
    emit NAME ## Changed(value); \
    plc1Proxy->writeData( m_ ## NAME ## _address , m_ ## NAME ## _value );\
    } \
    Q_SIGNAL void NAME ## Changed(int value);\
    private: \
    int m_ ## NAME ## _value; \
    QString m_ ## NAME ## _address = ADDRESS ;

#define CALL_THIS_IN_CONSTRACTEUR_FOR_ZONE_MEMOIRE_AUTO_UPDATE \
    QTimer *_syncTimer ;\
    _syncTimer = new QTimer(this);\
    _syncTimer->setInterval(100);\
    _syncTimer->setSingleShot(false);\
    connect(this, SIGNAL(destroyed(QObject*)), _syncTimer, SLOT(deleteLater()));\
    connect(_syncTimer, &QTimer::timeout, [=]() {\
    qDebug() << Q_FUNC_INFO << "timeout " ;\
    for(int i = this->metaObject()->methodOffset(); \
    i < this->metaObject()->methodCount(); i++) {\
    if(this->metaObject()->method(i).name().contains("_update")) \
    this->metaObject()->invokeMethod(this, \
    this->metaObject()->method(i).name(),\
    Qt::DirectConnection);\
    }\
    isConnected(plc1Proxy->plcIsOk());\
    } );\
    _syncTimer->start();\

#endif // MYPROPERTYHELPER_H
