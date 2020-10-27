QT += core
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/cursorpositionmessage.h \
    $$PWD/documentmessage.h \
    $$PWD/editingmessage.h \
    $$PWD/header.h \
    $$PWD/messageType.h \
    $$PWD/openmessage.h \
    $$PWD/usermessage.h

SOURCES += \
    $$PWD/cursorpositionmessage.cpp \
    $$PWD/documentmessage.cpp \
    $$PWD/editingmessage.cpp \
    $$PWD/header.cpp \
    $$PWD/openmessage.cpp \
    $$PWD/usermessage.cpp
