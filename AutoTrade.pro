QT       += core gui network websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
# disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    network/accessmanager.cpp \
    network/ratelimitmanager.cpp \
    network/ratelimitrulemanager.cpp \
    poe/ninjaapi.cpp \
    poe/staticdata.cpp \
    poe/tradeapi.cpp \
    ui/models/itemlistmodel.cpp

HEADERS += \
    mainwindow.hh \
    network/accessmanager.hh \
    network/ratelimitmanager.hh \
    network/ratelimitrulemanager.hh \
    poe/constants.hh \
    poe/ninjaapi.hh \
    poe/staticdata.hh \
    poe/tradeapi.hh \
    poe/types.hh \
    ui/models/itemlistmodel.hh

FORMS += \
    mainwindow.ui

LIBS += -lUser32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
