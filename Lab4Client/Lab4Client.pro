#-------------------------------------------------
#
# Project created by QtCreator 2016-10-09T18:16:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lab4Client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Actors/actor.cpp \
    Data/world.cpp \
    Client/client.cpp \
    gamepanel.cpp

HEADERS  += mainwindow.h \
    structs.h \
    Actors/actor.h \
    Data/world.h \
    Client/client.h \
    gamepanel.h

FORMS    += mainwindow.ui
