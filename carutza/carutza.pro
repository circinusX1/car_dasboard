#-------------------------------------------------
#
# Project created by QtCreator 2013-08-25T00:22:18
#
#-------------------------------------------------

QT       += core gui dbus script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += QT3_SUPPORT
RESOURCES =

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
