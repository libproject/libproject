#
# Finds QtCreator's source
# Input: none
# Provides:
# QTC_SRC - path to QtCreator's sources
#
# Usage sample:
# find_package(QtCSource)
# if (QTC_SRC_FOUND)
#  do_stuff...
# endif(QTC_SRC_FOUND)
#
set(PATH_TO_QTC_SOURCE "" CACHE PATH "Defined by user QtCreator's sources path")

if (NOT "${PATH_TO_QTC_SOURCE}" STREQUAL "")
  set(QTC_SRC ${PATH_TO_QTC_SOURCE} CACHE PATH "QtCreator's sources path")
  set(QTC_SRC_FOUND TRUE)
else ()
  if (NOT "$ENV{QTC_SOURCE}" STREQUAL "")
    set(QTC_SRC $ENV{QTC_SOURCE} CACHE PATH "QtCreator's sources path")
    set(QTC_SRC_FOUND TRUE)
  else ()
    set(PATH_TO_QTC_SOURCE_ALONG_WITH_PROJECT "${CMAKE_SOURCE_DIR}/../qt-creator")
    message(STATUS "QTC_SOURCE environment variable missing. QtCreator's sources will be searched at \"${CMAKE_SOURCE_DIR}/../qt-creator\"")
    find_path(QTC_SRC_PROJECT_FILE qtcreator.pro HINTS ${PATH_TO_QTC_SOURCE_ALONG_WITH_PROJECT})
    if (NOT QTC_SRC_PROJECT_FILE)
      message(FATAL "QtCreator sources weren't found")
      set(QTC_SRC_FOUND FALSE)
    else ()
      set(QTC_SRC ${PATH_TO_QTC_SOURCE_ALONG_WITH_PROJECT} CACHE PATH "QtCreator's sources path")
      set(QTC_SRC_FOUND TRUE)
    endif ()
  endif ()
endif ()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(QtCSource DEFAULT_MSG QTC_SRC)
