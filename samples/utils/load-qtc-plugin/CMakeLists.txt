cmake_minimum_required (VERSION 2.8.11)

#setting name
project (load_plugin)

#setting sources of app
set (SOURCES
        load_plugin.cpp)

#looking for Qt libs
find_package(Qt5CMakeModules)

#looking for QtCreator sources
find_package(QtCSource)

#looking for QtCreator build
find_package(QtC)

#setting necessary paths to CMake modules provided by Qt
set(Qt5Core_DIR "${QT_CMAKE_MODULES}/Qt5Core")
set(Qt5_DIR "${QT_CMAKE_MODULES}/Qt5")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${Qt5_DIR})

find_package(Qt5 COMPONENTS Core)

#setting necessary headers for PluginManager class
include_directories (
        ${QTC_SRC}/src/libs/
#        ${QTC_SRC}/src/libs/extensionsystem/
#        ${QTC_SRC}/src/libs/aggregation/
)

#setting what to build
add_executable(load_plugin ${SOURCES})

#linking with Qt and QtCreator libs
target_link_libraries(load_plugin Qt5::Core)
target_link_libraries(load_plugin ${QTC_BLD_CORELIB})
target_link_libraries(load_plugin ${QTC_BLD_AGGREGATIONLIB})
target_link_libraries(load_plugin ${QTC_BLD_EXTSYSLIB})
