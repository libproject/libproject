
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += qt

SOURCES += filesettest.cpp

include(deployment.pri)
qtcAddDeployment()

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../src/fileset/release/ -lproject
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../src/fileset/debug/ -lproject
else:unix: LIBS += -L$$OUT_PWD/../../src/fileset/ -lproject

INCLUDEPATH += $$PWD/../../src/fileset
DEPENDPATH += $$PWD/../../src/fileset

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -lgtest
QMAKE_CXXFLAGS += -lpthread
