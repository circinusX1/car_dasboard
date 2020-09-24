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

DISTFILES += \
    ../bin/Desktop/1nav.desktop \
    ../bin/Desktop/2media.desktop \
    ../bin/Desktop/3phone.desktop \
    ../bin/Desktop/4rearview.desktop \
    ../bin/Desktop/4rearview.desktop-disabled \
    ../bin/Desktop/4rearview.~desktop \
    ../bin/Desktop/audacious.widget \
    ../bin/Desktop/bt.widget \
    ../bin/Desktop/calc.widget \
    ../bin/Desktop/firefox.widget \
    ../bin/Desktop/firefox.widget.save \
    ../bin/Desktop/ioc.widget \
    ../bin/Desktop/mplayer.widget \
    ../bin/Desktop/op_pview.widget \
    ../bin/Desktop/vnc.widget \
    ../bin/Desktop/vnc.widget.txt \
    ../bin/Desktop/volume.widget \
    ../bin/Desktop/xfce.widget \
    ../bin/config/bottompanel/1x.signal \
    ../bin/config/bottompanel/2climate.desktop \
    ../bin/config/bottompanel/3controls.desktop \
    ../bin/config/bottompanel/4volume.desktop \
    ../bin/config/carutza.kiss \
    ../bin/config/desktop/1left.control \
    ../bin/config/desktop/2right.control \
    ../bin/config/desktop/camera.desktopp \
    ../bin/config/desktop/desktop.conf \
    ../bin/config/desktop/face.-desktopp \
    ../bin/config/desktop/facebook.-desktopp \
    ../bin/config/desktop/linedin.desktopp \
    ../bin/config/desktop/pandora.~desktop \
    ../bin/config/desktop/twiter.desktopp \
    ../bin/config/desktop/xm.desktop \
    ../bin/config/notifications/1clock.applet \
    ../bin/config/notifications/2weather.applet \
    ../bin/config/notifications/3bt.applet \
    ../bin/config/notifications/3wifi.applet \
    ../bin/config/notifications/4signal.applet \
    ../bin/config/notifications/5target.applet \
    ../bin/config/notifications/6user.applet \
    ../bin/config/op_ethd.conf \
    ../bin/config/overflow/1left.control \
    ../bin/config/overflow/2right.control \
    ../bin/config/overflow/3home.desktop \
    ../bin/config/overflow/camera.desktopp \
    ../bin/config/overflow/desktop.conf \
    ../bin/config/overflow/face.-desktopp \
    ../bin/config/overflow/facebook.-desktopp \
    ../bin/config/overflow/linedin.desktopp \
    ../bin/config/overflow/music.desktop \
    ../bin/config/overflow/pandora.desktopp \
    ../bin/config/overflow/pandora.~desktop \
    ../bin/config/overflow/pin.desktopp \
    ../bin/config/overflow/rssnews.desktopp \
    ../bin/config/overflow/twiter .~desktopp \
    ../bin/config/overflow/xm.desktop



