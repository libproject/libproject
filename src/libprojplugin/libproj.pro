DEFINES += LIBPROJ_LIBRARY

# libproj files

SOURCES += libprojplugin.cpp \
    libprojprojectmanager.cpp \
    libprojproject.cpp \
    libprojprojectnodes.cpp \
    libprojprojectfile.cpp


HEADERS += libprojplugin.h \
        libproj_global.h \
        libprojconstants.h \
    libprojprojectmanager.h \
    libprojproject.h \
    libprojprojectnodes.h \
    libprojprojectfile.h

# Qt Creator linking
## set the QTC_SOURCE environment variable to override the setting here
QTCREATOR_SOURCES = $$(QTC_SOURCE)
isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=/home/drew/Git/qt-creator

## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=/home/drew/QtCreatorProjects/QTC-GCC-BUILD

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%\QtProject\qtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/data/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator" on Linux
##    "~/Library/Application Support/QtProject/Qt Creator" on Mac
# USE_USER_DESTDIR = yes

###### If the plugin can be depended upon by other plugins, this code needs to be outsourced to
###### <dirname>_dependencies.pri, where <dirname> is the name of the directory containing the
###### plugin's sources.

QTC_PLUGIN_NAME = libproj
QTC_LIB_DEPENDS += \
    # nothing here at this time

QTC_PLUGIN_DEPENDS += \
    coreplugin \
    projectexplorer

QTC_PLUGIN_RECOMMENDS += \
    # optional plugin dependencies. nothing here at this time

###### End _dependencies.pri contents ######
include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)
include($$PWD/../../tools/json11.pri)
DEFINES -= QT_NO_CAST_FROM_ASCII
