#-------------------------------------------------
#
# Project created by QtCreator 2015-03-06T20:03:29
#
#-------------------------------------------------

TARGET = project
TEMPLATE = lib

DEFINES += LIBPROJECT_LIBRARY

SOURCES += libproject.cpp

HEADERS += libproject.h \
    libproject_error.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += ../../tools/json/src

QMAKE_CXXFLAGS += -std=c++11 -O0

QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage -fprofile-dir=$$OUT_PWD
QMAKE_LFLAGS += -fprofile-arcs -ftest-coverage -fprofile-dir=$$OUT_PWD
