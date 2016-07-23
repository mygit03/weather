#-------------------------------------------------
#
# Project created by QtCreator 2014-06-01T22:03:23
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += xml

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = weather
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    citymanager.cpp

HEADERS  += dialog.h \
    citymanager.h

FORMS    += dialog.ui

RESOURCES += \
    rc.qrc
