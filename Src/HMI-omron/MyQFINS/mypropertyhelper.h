#ifndef MYPROPERTYHELPER_H
#define MYPROPERTYHELPER_H

#pragma once
#include <QObject>
#include <QString>

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
       void NAME(int value) { \
          if (m_ ## NAME ## _value == value)  return; \
          m_ ## NAME ## _value = value; \
          emit NAME ## Changed(value); \
          myQFINS->writeData( m_ ## NAME ## _address , m_ ## NAME ## _value );\
        } \
       Q_SIGNAL void NAME ## Changed(int value);\
    private: \
       int m_ ## NAME ## _value; \
       QString m_ ## NAME ## _address = ADDRESS ;


#endif // MYPROPERTYHELPER_H
