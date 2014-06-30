#-------------------------------------------------
#
# Project created by QtCreator 2014-06-30T11:31:13
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = netroute
TEMPLATE = app


SOURCES += mainwindow.cpp \
    nodeitem.cpp \
    main.cpp \
    edgeitem.cpp \
    controlsdockwidget.cpp

HEADERS  += mainwindow.h \
    nodeitem.h \
    graph.h \
    edgeitem.h \
    controlsdockwidget.h

FORMS    += mainwindow.ui \
    controlsdockwidget.ui

RESOURCES += \
    netroute.qrc
