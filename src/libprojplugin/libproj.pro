DEFINES += LIBPROJ_LIBRARY

# libproj files

SOURCES += libprojplugin.cpp \
    libprojprojectmanager.cpp \
    libprojproject.cpp \
    libprojprojectnodes.cpp \
    libprojprojectfile.cpp \
    libprojfilesetloaders.cpp


HEADERS += libprojplugin.h \
        libproj_global.h \
        libprojconstants.h \
    libprojproject.h \
    libprojprojectnodes.h \
    libprojprojectfile.h \
    libprojprojectmanager.h \
    libprojfilesetloaders.h



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
DEFINES -= QT_NO_CAST_FROM_ASCII

RESOURCES += \
    libprojplugin.qrc

OTHER_FILES += \
    ../libprojw/wizard.json \
#    ../libprojw/file.libproject \
#    ../libprojw/file.h \
#    ../libprojw/main.cpp

#Copying wizard file
copydata.commands = $(COPY_DIR) $$PWD/../libprojw $$IDE_BUILD_TREE/share/qtcreator/templates/wizards/projects/
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../fileset/release/ -lfileset
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../fileset/debug/ -lfileset
else:unix: LIBS += -L$$OUT_PWD/../fileset/ -lfileset

INCLUDEPATH += $$PWD/../fileset
DEPENDPATH += $$PWD/../fileset

copydata.commands = $(COPY) $$OUT_PWD/../fileset/libfileset.so.1.0.0 $$IDE_BUILD_TREE/lib/qtcreator/plugins/libfileset.so.1
