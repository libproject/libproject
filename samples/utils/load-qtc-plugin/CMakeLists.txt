cmake_minimum_required (VERSION 2.8)

#setting name
project (load_plugin)

#setting variable for QtCreator's sources path
set(QTC_SRC NO CACHE PATH "QtCreator's sources path")

if (QTC_SRC)
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

else (QTC_SRC)
    message("QtCreator sources not defined!..")
endif(QTC_SRC)
