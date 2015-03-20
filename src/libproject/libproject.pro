#-------------------------------------------------
#
# Project created by QtCreator 2015-03-06T20:03:29
#
#-------------------------------------------------

QT       += core

TARGET = project
TEMPLATE = lib

DEFINES += LIBPROJECT_LIBRARY

SOURCES += libproject.cpp

HEADERS += libproject.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include($$PWD/../../tools/json11.pri)

QMAKE_CXXFLAGS += -std=c++11

QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage -fprofile-dir=$$OUT_PWD
QMAKE_LFLAGS += -fprofile-arcs -ftest-coverage -fprofile-dir=$$OUT_PWD
