CONFIG += ordered

TEMPLATE = subdirs

SUBDIRS += libproj_test src/libproj.pro

## set the QTC_SOURCE environment variable to override the setting here
QTCREATOR_SOURCES = $$(QTC_SOURCE)
isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=/home/drew/Git/qt-creator

## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=/home/drew/QtCreatorWorkBuild

LIBS +=  CoreFoundation

DEFINES += WITH_TESTS
