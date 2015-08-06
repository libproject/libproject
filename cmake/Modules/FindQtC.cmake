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
set(PATH_TO_QTC_BUILD "" CACHE PATH "Defined by user path to QtCreator build")

if (NOT "${PATH_TO_QTC_BUILD}" STREQUAL "")
  set(QTC_BLD ${PATH_TO_QTC_BUILD} CACHE PATH "QtCreator build path")
  set(QTC_BLD_FOUND TRUE)
else ()
  if (NOT "$ENV{QTC_BUILD}" STREQUAL "")
    set(QTC_BLD $ENV{QTC_BUILD} CACHE PATH "QtCreator build path")
    set(QTC_BLD_FOUND TRUE)
  else ()
    set(PATH_TO_QTC_BUILD_BINARY_ALONG_WITH_PROJECT "${CMAKE_SOURCE_DIR}/../qt-creator-build/bin")
    message(STATUS "QTC_BUILD environment variable missing. QtCreator build will be searched at \"${CMAKE_SOURCE_DIR}/../qt-creator-build\"")
    if (UNIX OR APPLE)
      find_path(QTC_BLD_BINARY qtcreator HINTS ${PATH_TO_QTC_BUILD_BINARY_ALONG_WITH_PROJECT})
    else ()
      find_path(QTC_BLD_BINARY qtcreator.exe HINTS ${PATH_TO_QTC_BUILD_BINARY_ALONG_WITH_PROJECT})
    endif ()
    if (NOT QTC_BLD_BINARY)
      message(FATAL "QtCreator build wasn't found")
      set(QTC_BLD_FOUND FALSE)
    else ()
      set(QTC_BLD "${CMAKE_SOURCE_DIR}/../qt-creator-build" CACHE PATH "QtCreator build path")
      set(QTC_BLD_FOUND TRUE)
    endif ()
  endif ()
endif ()

if(QTC_BLD_FOUND)
  if (UNIX OR APPLE)
    set(QTC_BLD_CORELIB "${QTC_BLD}/lib/qtcreator/plugins/libCore.so" CACHE FILEPATH "Path to core library")
    set(QTC_BLD_AGGREGATIONLIB "${QTC_BLD}/lib/qtcreator/libAggregation.so" CACHE FILEPATH "Path to aggregation library")
    set(QTC_BLD_EXTSYSLIB "${QTC_BLD}/lib/qtcreator/libExtensionSystem.so" CACHE FILEPATH "Path to extension system library")
  elseif ()
    set(QTC_BLD_CORELIB "${QTC_BLD}/lib/qtcreator/plugins/libCore.dll" CACHE FILEPATH "Path to core library")
    set(QTC_BLD_AGGREGATIONLIB "${QTC_BLD}/lib/qtcreator/libAggregation.dll" CACHE FILEPATH "Path to aggregation library")
    set(QTC_BLD_EXTSYSLIB "${QTC_BLD}/lib/qtcreator/libExtensionSystem.dll" CACHE FILEPATH "Path to extension system library")
  endif ()
endif ()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(QtC DEFAULT_MSG
QTC_BLD
QTC_BLD_BINARY
QTC_BLD_CORELIB
QTC_BLD_EXTSYSLIB
)
