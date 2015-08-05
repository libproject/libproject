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

if (NOT DEFINED $ENV{QTC_SOURCE})
  message (STATUS "System lacks of variable to QtC source. Starting to seeking, make sure you have sources in folder named `qt-creator` along with `libproject`")
  find_path(QTC_SRC_PROJECT_FILE qtcreator.pro HINTS "${CMAKE_SOURCE_DIR}/../qt-creator")
  if (NOT QTC_SRC_PROJECT_FILE)
    message(FATAL "QtCreator sources weren't found")
    set(QTC_SRC_FOUND FALSE)
  else (NOT QTC_SRC_PROJECT_FILE)
    set(QTC_SRC "${CMAKE_SOURCE_DIR}/../qt-creator" CACHE PATH "QtCreator's sources path")
    set(QTC_SRC_FOUND TRUE)
  endif (NOT QTC_SRC_PROJECT_FILE)
endif(NOT DEFINED $ENV{QTC_SOURCE})

if(DEFINED $ENV{QTC_SOURCE})
  set(QTC_SRC $ENV{QTC_SOURCE} CACHE PATH "QtCreator's sources path")
  set(QTC_SRC_FOUND TRUE)
endif(DEFINED $ENV{QTC_SOURCE})

message(STATUS "QtCreator's sources found")

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(QtCSource DEFAULT_MSG QTC_SRC)
