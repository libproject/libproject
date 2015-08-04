cmake_minimum_required (VERSION 2.8.11)

#setting name
project (load_plugin)

#setting variable for QtCreator's sources path
set(QTC_SRC NO CACHE PATH "QtCreator's sources path")

set(QT_CMAKE_MODULES NO CACHE PATH "CMake's modules which provided by Qt itself for Qt bindings")

if (QTC_SRC AND QT_CMAKE_MODULES)
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

    #setting sources
    set (SOURCES
            load_plugin.cpp)

    #setting what to build
    add_executable(load_plugin ${SOURCES})

    target_link_libraries(load_plugin Qt5::Core)

else (QTC_SRC AND QT_CMAKE_MODULES)
    message("Paths to QtCreator sources or/and Qt-CMake modules not defined!..")
endif(QTC_SRC AND QT_CMAKE_MODULES)
