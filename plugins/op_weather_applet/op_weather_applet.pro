#-------------------------------------------------
#
# Project created by QtCreator 2013-09-17T14:07:41
#
#-------------------------------------------------


QT       += core gui dbus script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = op_clock_applet
TEMPLATE = app


SOURCES += main.cpp\
        clockwindow.cpp \
    ../include/dashdb_proxy.cpp

HEADERS  += clockwindow.h \
    ../include/dashdb_proxy.h

FORMS    += clockwindow.ui

OTHER_FILES += \
    ../op_dash/dash/dashdb.xml

#-------------------------------------------------
#
# Project created by QtCreator 2013-09-17T14:07:41
#
#-------------------------------------------------
