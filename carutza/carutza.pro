#-------------------------------------------------
#
# Project created by QtCreator 2013-08-25T00:22:18
#
#-------------------------------------------------

QT       += core gui dbus script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += QT3_SUPPORT
RESOURCES =
QMAKE_CXXFLAGS += -std=c++17
UI_HEADERS_DIR = forms
FORMS += \
    fxabout.ui

OBJECTS_DIR += ../build
MOC_DIR += ./
QMAKE_INCDIR += /usr/include forms/
QMAKE_CLEAN += carutza


TARGET = carutza
TEMPLATE = app

SOURCES += main.cpp\
    akajson.cpp \
    ctrlholder.cpp \
    desktop.cpp \
    menu.cpp \
    button.cpp \
    client.cpp \
    imagez.cpp \
    mysett.cpp \
    theapp.cpp \
    wxframe.cpp \
    lunchbutt.cpp \
    qwidgeticon.cpp \
    dskwatch.cpp \
    appman.cpp \
    taskswitch.cpp \
    x11atoms.cpp \
    panel.cpp \
    qslunchbutt.cpp \
    dbusipc.cpp \
    dashdb_iface.cpp

HEADERS  += \
    akajson.h \
    ctrlholder.h \
    desktop.h \
    defs.h \
    menu.h \
    client.h \
    button.h \
    imagez.h \
    mysett.h \
    forms/ui_frm_quit.h \
    forms/ui_frm_about.h \
    theapp.h \
    wxframe.h \
    lunchbutt.h \
    qwidgeticon.h \
    events.h \
    dskwatch.h \
    appman.h \
    taskswitch.h \
    x11atoms.h \
    panel.h \
    qslunchbutt.h \
    dbusipc.h \
    dashdb_iface.h


unix|win32: LIBS += -lX11


FORMS    +=

OTHER_FILES += \

DISTFILES += \ \
    ../bin/CONFIG/MENU/Apps/desktop.conf \
    ../bin/CONFIG/MENU/Apps/maps.desktop \
    ../bin/CONFIG/MENU/Apps/rssnews.desktop \
    ../bin/CONFIG/MENU/Apps/xm.desktop \
    ../bin/CONFIG/MENU/Car/camera.desktop \
    ../bin/CONFIG/MENU/Car/desktop.conf \
    ../bin/CONFIG/MENU/Car/engine.desktop \
    ../bin/CONFIG/MENU/Car/oil.desktop \
    ../bin/CONFIG/MENU/Car/tires.desktop \
    ../bin/CONFIG/MENU/Car/water.desktop \
    ../bin/CONFIG/MENU/Media/desktop.conf \
    ../bin/CONFIG/MENU/Media/music.desktop \
    ../bin/CONFIG/MENU/Media/radio.desktop \
    ../bin/CONFIG/MENU/System/desktop.conf \
    ../bin/CONFIG/MENU/System/maps.desktop \
    ../bin/CONFIG/MENU/System/music.desktop \
    ../bin/CONFIG/TOP/2weather.applet \
    ../bin/CONFIG/TOP/3bt.applet \
    ../bin/CONFIG/TOP/3wifi.applet \
    ../bin/CONFIG/TOP/4signal.applet \
    ../bin/CONFIG/TOP/5target.applet \
    ../bin/CONFIG/TOP/8clock.applet \
    ../bin/DESKTOP/Apps/desktop.conf \
    ../bin/DESKTOP/Apps/pandora.desktop \
    ../bin/DESKTOP/Apps/rssnews.desktop \
    ../bin/DESKTOP/Apps/xm.desktop \
    ../bin/carutza.kiss \
    ../bin/main/1left.control \
    ../bin/main/2right.control \
    ../bin/main/camera.desktopp \
    ../bin/main/desktop.conf \
    ../bin/main/face.-desktopp \
    ../bin/main/facebook.-desktopp \
    ../bin/main/linedin.desktopp \
    ../bin/main/pandora.~desktop \
    ../bin/main/twiter.desktopp \
    ../bin/main/xm.desktop \
    ../bin/notifications/2weather.applet \
    ../bin/notifications/3bt.applet \
    ../bin/notifications/3wifi.applet \
    ../bin/notifications/4signal.applet \
    ../bin/notifications/5target.applet \
    ../bin/notifications/8clock.applet \
    ../bin/overflow/1nav.desktop \
    ../bin/overflow/camera.desktop \
    ../bin/overflow/desktop.conf \
    ../bin/overflow/home.control \
    ../bin/overflow/linedin.desktop \
    ../bin/overflow/maps.desktop \
    ../bin/overflow/music.desktop \
    ../bin/overflow/pandora.desktop \
    ../bin/overflow/rssnews.desktop \
    ../bin/overflow/xm.desktop
