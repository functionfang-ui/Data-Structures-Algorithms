#-------------------------------------------------
#
# Project created by QtCreator 2013-08-22T22:46:33
#
#-------------------------------------------------

QT       += core gui multimedia

CONFIG += c++11

CONFIG += resources_big

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TowerDefense
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    towerposition.cpp \
    waypoint.cpp \
    enemy.cpp \
    audioplayer.cpp \
    plistreader.cpp \
    arrow.cpp \
    cupola.cpp \
    bomb.cpp \
    arrowtower.cpp

HEADERS  += mainwindow.h \
    towerposition.h \
    waypoint.h \
    utility.h \
    enemy.h \
    audioplayer.h \
    plistreader.h \
    arrow.h \
    cupola.h \
    bomb.h \
    arrowtower.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
