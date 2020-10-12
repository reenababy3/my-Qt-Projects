#-------------------------------------------------
#
# Project created by QtCreator 2020-09-12T17:39:17
#
#-------------------------------------------------

QT       += core gui
CONFIG  += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Load_File
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    processfile.cpp \
    highlightletters.cpp

HEADERS  += mainwindow.h \
    processfile.h \
    highlightletters.h

FORMS    += mainwindow.ui
