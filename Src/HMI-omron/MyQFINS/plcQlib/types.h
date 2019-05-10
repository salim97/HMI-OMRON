#ifndef TYPES_H
#define TYPES_H

#include <QMetaType>
#include <QVector>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

Q_DECLARE_METATYPE( u16)

Q_DECLARE_METATYPE( QList<u16>)


#endif // TYPES_H
