#
# Finds QtCreator build
# Input: none
# Provides:
# QTC_BLD - path to QtCreator build
# QTC_BLD_BINARY - path to main executable of QtCreator
# QTC_BLD_CORELIB - path to core library of QtCreator
# QTC_BLD_AGGREGATIONLIB - Path to aggregation library of QtCreator
# QTC_BLD_EXTSYSLIB - Path to extension system library of QtCreator
#
# Usage sample:
# find_package(QtC)
# if (QTC_BLD_FOUND)
#  do_some_stuff...
# endif(QTC_BLD_FOUND)
# target_link_libraries(<some_target> ${QTC_BLD_CORELIB})
#
if ("$ENV{QTC_BUILD}" STREQUAL "")
  message (STATUS "System lacks of variable to QtC build. Starting to seeking, make sure you have build in folder named `qt-creator-build` along with `libproject`")
  if (UNIX OR APPLE)
    find_path(QTC_BLD_BINARY qtcreator HINTS "${CMAKE_SOURCE_DIR}/../qt-creator-build/bin")
  else (UNIX OR APPLE)
    find_path(QTC_BLD_BINARY qtcreator.exe HINTS "${CMAKE_SOURCE_DIR}/../qt-creator-build/bin")
  endif(UNIX OR APPLE)

  if (NOT QTC_BLD_BINARY)
    message(FATAL "QtCreator build weren't found")
    set(QTC_BLD_FOUND FALSE)
  else (NOT QTC_BLD_BINARY)
    set(QTC_BLD "${CMAKE_SOURCE_DIR}/../qt-creator-build" CACHE PATH "QtCreator build path")
    set(QTC_BLD_FOUND TRUE)
    message(STATUS "QtCreator build is found")
  endif (NOT QTC_BLD_BINARY)
endif("$ENV{QTC_BUILD}" STREQUAL "")

if(NOT "$ENV{QTC_BUILD}" STREQUAL "")
  set(QTC_BLD $ENV{QTC_BUILD} CACHE PATH "QtCreator's sources path")
  set(QTC_BLD_FOUND TRUE)
endif(NOT "$ENV{QTC_BUILD}" STREQUAL "")

if(QTC_BLD_FOUND)
  if (UNIX OR APPLE)
    set(QTC_BLD_CORELIB "${QTC_BLD}/lib/qtcreator/plugins/libCore.so" CACHE FILEPATH "Path to core library")
    set(QTC_BLD_AGGREGATIONLIB "${QTC_BLD}/lib/qtcreator/libAggregation.so" CACHE FILEPATH "Path to aggregation library")
    set(QTC_BLD_EXTSYSLIB "${QTC_BLD}/lib/qtcreator/libExtensionSystem.so" CACHE FILEPATH "Path to extension system library")
  elseif (UNIX OR APPLE)
    set(QTC_BLD_CORELIB "${QTC_BLD}/lib/qtcreator/plugins/libCore.dll" CACHE FILEPATH "Path to core library")
    set(QTC_BLD_AGGREGATIONLIB "${QTC_BLD}/lib/qtcreator/libAggregation.dll" CACHE FILEPATH "Path to aggregation library")
    set(QTC_BLD_EXTSYSLIB "${QTC_BLD}/lib/qtcreator/libExtensionSystem.dll" CACHE FILEPATH "Path to extension system library")
  endif(UNIX OR APPLE)
endif(QTC_BLD_FOUND)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(QtC DEFAULT_MSG
QTC_BLD
QTC_BLD_BINARY
QTC_BLD_CORELIB
QTC_BLD_EXTSYSLIB
)
