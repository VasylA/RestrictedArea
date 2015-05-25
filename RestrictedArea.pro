#-------------------------------------------------
#
# Project created by QtCreator 2015-02-11T21:02:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RestrictedArea
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    gscene.cpp \
    grectitem.cpp \
    draggableitem.cpp \
    gview.cpp \
    glabel.cpp

HEADERS  += mainwindow.h \
    gscene.h \
    grectitem.h \
    draggableitem.h \
    gview.h \
    glabel.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc
