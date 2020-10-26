QT += network
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/tcpconnection.h \
    $$PWD/tcpconnections.h \
    $$PWD/tcpserver.h

SOURCES += \
    $$PWD/tcpconnection.cpp \
    $$PWD/tcpconnections.cpp \
    $$PWD/tcpserver.cpp
