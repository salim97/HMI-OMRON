#ifndef MYPROPERTYHELPER_H
#define MYPROPERTYHELPER_H

#pragma once
#include <QObject>
#include <QString>
#include <QTimer>
#include <QMetaMethod>
#include <QSettings>

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

#define CALL_THIS_IN_CONSTRACTEUR_FOR_AUTO_PROPERTY_SETTINGS \
for(int i = this->metaObject()->methodOffset(); \
    i < this->metaObject()->methodCount(); i++) \
{ \
    if(this->metaObject()->method(i).name().contains("init")) \
        this->metaObject()->invokeMethod(this, \
                                         this->metaObject()->method(i).name(),\
                                         Qt::DirectConnection);\
}\

#define ZONE_MEMOIRE_SETTINGS(ADDRESS, NAME) \
    Q_PROPERTY(int NAME READ NAME WRITE NAME NOTIFY NAME ## Changed ) \
    public: \
    int NAME() const { return m_ ## NAME ## _value ; } \
    Q_INVOKABLE void NAME ## _read() {\
    NAME(plc1Proxy->readProxyData(m_ ## NAME ## _address));\
    } \
    Q_INVOKABLE QString NAME ## _toBIN_asSTRING() {\
    QString s = QString::number(  m_ ## NAME ## _value , 2 );\
    std::reverse(s.begin(), s.end());\
    for( int i = s.length() ; s.length() < 16 ; i++ )\
        s.append("0");\
    return  s;\
    } \
    Q_INVOKABLE QList<bool> NAME ## _toBIN_asBOOL() {\
    QString s = QString::number(  m_ ## NAME ## _value , 2 );\
    std::reverse(s.begin(), s.end());\
    for( int i = s.length() ; s.length() < 16 ; i++ )\
        s.append("0");\
    QList<bool> r ;\
    for( int i = 0 ; i < s.length() ; i++ )\
        if(s[i] == "0")\
            r << false ;\
        else\
            r << true ; \
    return  r;\
    } \
    Q_INVOKABLE void NAME ## _send() {\
    plc1Proxy->writeData( m_ ## NAME ## _address , m_ ## NAME ## _value );\
    } \
    Q_INVOKABLE void init ## NAME() \
    {\
        QString func_name = Q_FUNC_INFO;\
        func_name = func_name.split("(")[0];\
        func_name = func_name.split("::")[1];\
        func_name.remove(0, 4 );\
        QSettings settings("myapp.ini", QSettings ::IniFormat );\
        NAME( qvariant_cast<int>(settings.value(func_name)) ) ;     \
    }\
    void NAME(int value) { \
    if (m_ ## NAME ## _value == value)  return; \
    QString func_name = Q_FUNC_INFO;\
    func_name = func_name.split("(")[0];\
    func_name = func_name.split("::")[1];\
    QSettings settings("myapp.ini", QSettings::IniFormat );\
    settings.setValue(func_name , value  );\
    m_ ## NAME ## _value = value; \
    emit NAME ## Changed(value); \
    } \
    Q_SIGNAL void NAME ## Changed(int value);\
    private: \
    int m_ ## NAME ## _value; \
    QString m_ ## NAME ## _address = ADDRESS ;

#define ZONE_MEMOIRE(ADDRESS, NAME) \
    Q_PROPERTY(int NAME READ NAME WRITE NAME NOTIFY NAME ## Changed ) \
    public: \
    int NAME() const { return m_ ## NAME ## _value ; } \
    Q_INVOKABLE void NAME ## _read() {\
    NAME(plc1Proxy->readProxyData(m_ ## NAME ## _address));\
    } \
    Q_INVOKABLE QString NAME ## _toBIN_asSTRING() {\
    QString s = QString::number(  m_ ## NAME ## _value , 2 );\
    std::reverse(s.begin(), s.end());\
    for( int i = s.length() ; s.length() < 16 ; i++ )\
        s.append("0");\
    return  s;\
    } \
    Q_INVOKABLE QList<bool> NAME ## _toBIN_asBOOL() {\
    QString s = QString::number(  m_ ## NAME ## _value , 2 );\
    std::reverse(s.begin(), s.end());\
    for( int i = s.length() ; s.length() < 16 ; i++ )\
        s.append("0");\
    QList<bool> r ;\
    for( int i = 0 ; i < s.length() ; i++ )\
        if(s[i] == "0")\
            r << false ;\
        else\
            r << true ; \
    return  r;\
    } \
    Q_INVOKABLE void NAME ## _send() {\
    plc1Proxy->writeData( m_ ## NAME ## _address , m_ ## NAME ## _value );\
    } \
    void NAME(int value) { \
    if (m_ ## NAME ## _value == value)  return; \
    m_ ## NAME ## _value = value; \
    emit NAME ## Changed(value); \
    } \
    Q_SIGNAL void NAME ## Changed(int value);\
    private: \
    int m_ ## NAME ## _value; \
    QString m_ ## NAME ## _address = ADDRESS ;

#define CALL_THIS_IN_CONSTRACTEUR_FOR_ZONE_MEMOIRE_AUTO_UPDATE \
    QTimer *_syncTimer ;\
    _syncTimer = new QTimer(this);\
    _syncTimer->setInterval(500);\
    _syncTimer->setSingleShot(false);\
    connect(this, SIGNAL(destroyed(QObject*)), _syncTimer, SLOT(deleteLater()));\
    connect(_syncTimer, &QTimer::timeout, [=]() {\
    qDebug() << Q_FUNC_INFO << "timeout " ;\
    for(int i = this->metaObject()->methodOffset(); \
    i < this->metaObject()->methodCount(); i++) {\
    if(this->metaObject()->method(i).name().contains("_read")) \
    this->metaObject()->invokeMethod(this, \
    this->metaObject()->method(i).name(),\
    Qt::DirectConnection);\
    }\
    isConnected(plc1Proxy->plcIsOk());\
    } );\
    _syncTimer->start();\

#endif // MYPROPERTYHELPER_H
