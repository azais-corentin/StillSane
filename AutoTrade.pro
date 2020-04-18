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
    craft/crafter.cpp \
    main.cpp \
    mainwindow.cpp \
    poe/api/ninja.cpp \
    poe/api/trade.cpp \
    ui/delegates/checkbox.cpp \
    ui/luahighlighter.cpp \
    ui/models/itemlistmodel.cpp \
    ui/models/searchresulttree.cpp \
    ui/models/searchtable.cpp \
    network/accessmanager.cpp \
    network/ratelimitmanager.cpp \
    network/ratelimitrulemanager.cpp \
    poe/searchmanager.cpp \
    poe/api/staticdata.cpp

HEADERS += \
    craft/crafter.hh \
    craft/statemachines.hh \
    mainwindow.hh \
    poe/api/ninja.hh \
    poe/api/trade.hh \
    ui/delegates/checkbox.hh \
    ui/luahighlighter.hh \
    ui/models/itemlistmodel.hh \
    ui/models/searchresulttree.hh \
    ui/models/searchtable.hh \
    network/accessmanager.hh \
    network/ratelimitmanager.hh \
    network/ratelimitrulemanager.hh \
    poe/searchmanager.hh \
    poe/api/constants.hh \
    poe/api/staticdata.hh \
    poe/api/types.hh

FORMS += \
    mainwindow.ui

LIBS += -lUser32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Libraries
INCLUDEPATH += libs

# Lua
INCLUDEPATH += "C:\cpplibs\lua\include"
LIBS += -L"C:\cpplibs\lua\lib"
LIBS += -llua
