#-------------------------------------------------
#
# Project created by QtCreator 2016-05-24T00:25:26
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WormsRev_3
TEMPLATE = app




SOURCES += main.cpp\
        mainclass.cpp \
    bullet.cpp \
    worm.cpp \
    player.cpp \
    keyhandler.cpp \
    collisiondetection.cpp \
    gunlistdisplay.cpp \
    mylabel.cpp \
    mouseeventhandler.cpp \
    maplabel.cpp

HEADERS  += mainclass.h \
    worm.h \
    bullet.h \
    player.h \
    keyhandler.h \
    collisiondetection.h \
    gunlistdisplay.h \
    mylabel.h \
    mouseeventhandler.h \
    maplabel.h

FORMS    += mainclass.ui

RESOURCES += \
    resources.qrc
