INCLUDEPATH += $$PWD/plcQlib
include(plcQlib/plcQlib.pri)

HEADERS += \
    $$PWD/myqfins.h \
    $$PWD/myomron.h \
    $$PWD/mypropertyhelper.h

SOURCES += \
    $$PWD/myqfins.cpp \
    $$PWD/myomron.cpp
