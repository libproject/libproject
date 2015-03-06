#-------------------------------------------------
#
# Project created by QtCreator 2015-03-06T20:03:29
#
#-------------------------------------------------

QT       += core

TARGET = fileset
TEMPLATE = lib

DEFINES += FILESET_LIBRARY

SOURCES += fileset.cpp

HEADERS += fileset.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include($$PWD/../../tools/json11.pri)

QMAKE_CXXFLAGS += -std=c++11
