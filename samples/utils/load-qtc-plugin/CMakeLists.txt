cmake_minimum_required (VERSION 2.8.11)

#setting name
project (load_plugin)

#looking for Qt libs
set(QT_CMAKE_MODULES NO CACHE PATH "CMake's modules which provided by Qt itself for Qt bindings")

#looking for QtCreator sources
find_package(QtCSource)

#looking for QtCreator build
find_package(QtC)

#main part
if (QTC_SRC_FOUND AND QTC_BLD_FOUND AND QTC_CMAKE_MODULES)
    #setting necessary paths to CMake modules provided by Qt
    set(Qt5Core_DIR "${QT_CMAKE_MODULES}/Qt5Core")
    set(Qt5_DIR "${QT_CMAKE_MODULES}/Qt5")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${Qt5_DIR})

    find_package(Qt5 COMPONENTS Core)

    #setting necessary headers for PluginManager class
    include_directories (
            ${QTC_SRC}/src/libs/extensionsystem/
            ${QTC_SRC}/src/libs/aggregation/
    )

    #setting sources of app
    set (SOURCES
            load_plugin.cpp)

    #setting what to build
    add_executable(load_plugin ${SOURCES})

    #linking with Qt and QtCreator libs
    target_link_libraries(load_plugin Qt5::Core)
    target_link_libraries(load_plugin ${QTC_BLD_CORELIB})

else (QTC_SRC_FOUND AND QTC_BLD_FOUND AND QTC_CMAKE_MODULES)
    message("Seems you didn't specified path to Qt libs")
endif (QTC_SRC_FOUND AND QTC_BLD_FOUND AND QTC_CMAKE_MODULES)
