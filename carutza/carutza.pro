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
    desktop.cpp \
    menu.cpp \
    button.cpp \
    client.cpp \
    imagez.cpp \
    mysett.cpp \
    theapp.cpp \
    wxframe.cpp \
    lunchbutt.cpp \
    qwidget2.cpp \
    qwidgeticon.cpp \
    dskwatch.cpp \
    appman.cpp \
    taskswitch.cpp \
    x11atoms.cpp \
    panel.cpp \
    qslunchbutt.cpp \
    container.cpp \
    dbusipc.cpp \
    dashdb_iface.cpp

HEADERS  += \
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
    qwidget2.h \
    qwidgeticon.h \
    events.h \
    dskwatch.h \
    appman.h \
    taskswitch.h \
    x11atoms.h \
    panel.h \
    qslunchbutt.h \
    container.h \
    dbusipc.h \
    dashdb_iface.h


unix|win32: LIBS += -lX11


FORMS    +=

OTHER_FILES += \
    ../../../../.octopus/config/dashboard.conf \
    ../../../../.octopus/Desktop/vlc.widget \
    ../../../../.octopus/Desktop/media.desktop \
    ../../../../.octopus/Desktop/Settings/desktop.conf \
    ../../../../.octopus/Desktop/enna.desktop \
    ../../../../.octopus/Desktop/sample.desktop \
    ../../../../.octopus/Desktop/.notifications/bt.applet \
    ../../../../.octopus/Desktop/.notifications/clock.applet \
    ../../../../.octopus/Desktop/.notifications/clock.applet~ \
    ../../../../.octopus/Desktop/.notifications/target.applet \
    ../../../../.octopus/Desktop/.notifications/weather.applet \
    ../../../../.octopus/Desktop/.notifications/apps.applet \
    ../../../../.octopus/Desktop/.notifications/clock.qs \
    ../../../../.octopus/Desktop/Diagnostics/desktop.conf \
    ../../../../.octopus/Desktop/XDevices/desktop.conf \
    ../../../../.octopus/Desktop/Usbs/desktop.conf \
    ../../../../.octopus/Desktop/.overflow/home.control \
    ../../../../.octopus/Desktop/.overflow/left.control \
    ../../../../.octopus/Desktop/.overflow/right.control \
    ../../../../.octopus/Desktop/.overflow/xhome.control \
    dashdb.xml \
    ../../../../.octopus/Desktop/.notifications/wifii.applet \
    ../../../../.octopus/Desktop/.notifications/user.applet \
    ../../../../.octopus/Desktop/.notifications/cbt.applet \
    ../../system/home/dot.octopus/Desktop/.bottompanel/4volume.signal \
    ../../system/home/dot.octopus/Desktop/.bottompanel/1x.signal \
    ../../system/home/dot.octopus/Desktop/.bottompanel/3controls.desktop \
    ../../system/home/dot.octopus/Desktop/.bottompanel/2climate.desktop \
    ../../system/home/dot.octopus/Desktop/calc.widget \
    ../../system/home/dot.octopus/Desktop/vnc.widget \
    ../../system/home/dot.octopus/Desktop/3phone.desktop \
    ../../system/home/dot.octopus/Desktop/1nav.desktop \
    ../../../../.octopus/Desktop/.overflow/3home.desktop \
    ../../../../.octopus/Desktop/.overflow/home.control~ \
    ../../../../.octopus/Desktop/.overflow/2right.control \
    ../../../../.octopus/Desktop/.overflow/1left.control \
    ../../../../.octopus/Desktop/.overflow/2right.control~ \
    ../bin/config/carutza.conf \
    ../bin/Desktop/.bottompanel/1x.signal \
    ../bin/Desktop/.bottompanel/4volume.desktop \
    ../bin/Desktop/.bottompanel/3controls.desktop \
    ../bin/Desktop/.bottompanel/2climate.desktop \
    ../bin/Desktop/.overflow/3home.desktop \
    ../bin/Desktop/.overflow/2right.control \
    ../bin/Desktop/.overflow/2right.control~ \
    ../bin/Desktop/.overflow/home.control~ \
    ../bin/Desktop/.overflow/1left.control



