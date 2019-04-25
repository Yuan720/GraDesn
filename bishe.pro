#-------------------------------------------------
#
# Project created by QtCreator 2019-04-09T21:48:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets  printsupport

TARGET = bishe
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    cacularthread.cpp \
    bridge.cpp \
    cacularthread.cpp \
    MBIF.cpp \
    CarriagewayPlate.cpp \
    Casoa.cpp \
    mytoolkit.cpp \
    mywidget.cpp \
    XxwTracer.cpp \
    XxwTracer.cpp \
    XxwCustomPlot.cpp \
    qcustomplot.cpp \
    variables.cpp

HEADERS += \
        mainwindow.h \
    cacularthread.h \
    bridge.h \
    CarriagewayPlate.h \
    MBIF.h \
    Casoa.h \
    mytoolkit.h \
    mywidget.h \
    XxwCustomPlot.h \
    XxwTracer.h \
    qcustomplot.h \
    variables.h

FORMS += \
        mainwindow.ui
CONFIG +=C++11

DISTFILES +=

RESOURCES += \
    resource.qrc
RC_ICONS="myapp.ico"
