#-------------------------------------------------
#
# Project created by QtCreator 2020-09-30T14:57:06
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = justradio
TEMPLATE = app

INCLUDEPATH += ../libsdr
INCLUDEPATH += libusb-1.0

DEFINES += _CFG_GNU_LINUX
DEFINES +=  GL_GLEXT_PROTOTYPES

QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-unused-variable -Wno-sign-compare -Wno-unused-parameter -Wno-ignore-qualifiers

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        camera.cpp \
        convenience.cpp \
        frustum.cpp \
        glqgl.cpp \
        laout.cpp \
        main.cpp \
        dialog.cpp \
        playthread.cpp \
        sdrmain.cpp

HEADERS += \
        baselib.h \
        baseutils.h \
        camera.h \
        convenience.h \
        dialog.h \
        frustum.h \
        glqgl.h \
        laout.h \
        os.h \
        playthread.h \
        sdrmain.h

FORMS += \
        dialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


LIBS +=  -lusb-1.0
LIBS +=  -lrtlsdr
LIBS += -lglut -lGLU -lGL
LIBS +=  -lasound -lao

DISTFILES +=
