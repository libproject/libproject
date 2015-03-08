#-------------------------------------------------
#
# Project created by QtCreator 2015-03-08T14:12:59
#
#-------------------------------------------------

QT       -= core gui

TARGET = filesetmock
TEMPLATE = lib

DEFINES += FILESETMOCK_LIBRARY

SOURCES += filesetmock.cpp

HEADERS += filesetmock.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += $$PWD/../../src/fileset
DEPENDPATH += $$PWD/../../src/fileset

QMAKE_CXXFLAGS += -std=c++11
