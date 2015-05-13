
DEFINES += PROJ_LIBRARY

SOURCES += libprojplugin.cpp \
    libprojprojectmanager.cpp \
    libprojproject.cpp \
    libprojprojectnodes.cpp \
    libprojprojectfile.cpp


HEADERS += libprojplugin.h \
        libproj_global.h \
        libprojconstants.h \
    libprojproject.h \
    libprojprojectnodes.h \
    libprojprojectfile.h \
    libprojprojectmanager.h


# Qt Creator linking
## set the QTC_SOURCE environment variable to override the setting here
QTCREATOR_SOURCES = $$(QTC_SOURCE)
isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=/home/drew/Git/qt-creator
message(Path to QtC build: $$QTCREATOR_SOURCES)
## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=/home/drew/QtCreatorProjects/QTC34-GCC-BUILD
message(Path to QtC build: $$IDE_BUILD_TREE)

QTC_PLUGIN_NAME = proj
QTC_LIB_DEPENDS += \
    # nothing here at this time

QTC_PLUGIN_DEPENDS += \
    coreplugin \
    projectexplorer \
    qtsupport ###

QTC_PLUGIN_RECOMMENDS += \
    # optional plugin dependencies. nothing here at this time

###### End _dependencies.pri contents ######
include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)
DEFINES -= QT_NO_CAST_FROM_ASCII

RESOURCES += \
    libprojplugin.qrc

OTHER_FILES += \
    wizard/wizard.json

LIBS += -lproject

copydata.commands = $(COPY_DIR) $$PWD/wizard $$IDE_BUILD_TREE/share/qtcreator/templates/wizards/projects/libprojw

first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
