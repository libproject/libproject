
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += qt

## If environment variable GMOCK_HOME is set, get path here, otherwise get default value
GMOCK_HOME = $$(GMOCK_HOME)
isEmpty(GMOCK_HOME):GMOCK_HOME="$$PWD/../../../gmock"

SOURCES += filesettest.cpp \
           $$GMOCK_HOME/src/gmock-all.cc \
           $$GMOCK_HOME/gtest/src/gtest-all.cc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../src/fileset/release/ -lproject
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../src/fileset/debug/ -lproject
else:unix: LIBS += -L$$OUT_PWD/../../src/fileset/ -lproject

INCLUDEPATH += $$PWD/../../src/fileset \
               $$GMOCK_HOME/include \
               $$GMOCK_HOME/gtest/include \
               $$GMOCK_HOME/gtest \
               $$GMOCK_HOME/

DEPENDPATH += $$PWD/../../src/fileset

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -lpthread

copydata.commands = $(COPY_DIR) $$PWD/project_files $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

## Defining additional target for code coverage reports
#### .gcno and .gcda files in build directory generating every build cycle hence
#### there are three additional following parameters for c++-compiler

coverage.commands = \
(LD_LIBRARY_PATH=$$OUT_PWD/../../src/fileset/ ./filesettest ; \
cd $$OUT_PWD/../../src/fileset ; \
gcov -o . $$PWD/../../src/fileset/fileset.cpp ; \
gcovr --object-directory= . -r $$PWD/../../src/fileset/ --html --html-details -o $$OUT_PWD/report.html)
QMAKE_EXTRA_TARGETS += coverage
