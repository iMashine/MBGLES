#-------------------------------------------------
#
# Project created by QtCreator 2017-08-07T10:42:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = workWithGraphics
TEMPLATE = app

CONFIG += warn_off

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += include/_core
INCLUDEPATH += include/opengl
DEPENDPATH += $$PWD/include

LIBS += -L$$PWD/lib/ -lmagic
LIBS += -lOpengl32

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/lib/magic.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/lib/libmagic.a

SOURCES += \
        main.cpp \
        mainwidget.cpp \
    mainopenglwidget.cpp \
    emitter.cpp \
    ptextures.cpp \
    include/_core/mp.cpp \
    include/_core/platform_win_posix.cpp \
    emitterlist.cpp \
    emitterslistview.cpp \
    shaderprogram.cpp

HEADERS += \
        mainwidget.h \
    mainopenglwidget.h \
    emitter.h \
    ptextures.h \
    include/_core/magic.h \
    include/_core/mp.h \
    include/_core/platform_win_posix.h \
    emitterlist.h \
    emitterslistview.h \
    shaderprogram.h
