QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    db.cpp \
    DocumentEntity.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    DocumentEntity.h \
    db.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

include(network/network.pri)
include(network/messages/messages.pri)
include(common/common.pri)
include(server_editor/server_editor.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
