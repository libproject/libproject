#
# Finds Qt's CMake modules
# Input: none
# Provides:
# QT_CMAKE_MODULES - path to Qt's CMake modules
#
# Usage sample:
# find_package(Qt5CMakeModules)
# if (QT_CMAKE_MODULES_FOUND)
#  do_some_stuff...
# endif(QT_CMAKE_MODULES_FOUND)
#
if (NOT "$ENV{QT_LIBS_DIR}" STREQUAL "")
  set(QT_CMAKE_MODULES "$ENV{QT_LIBS_DIR}/lib/cmake" CACHE PATH "CMake's modules which provided by Qt itself for Qt bindings")
  set(QT_CMAKE_MODULES_FOUND TRUE)
  message(STATUS "Qt-CMake modules are found")
else()
  message(FATAL "You need to have system-wide variable with path to Qt libs")
  set(QT_CMAKE_MODULES_FOUND FALSE)
endif()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Qt5CMakeModules DEFAULT_MSG QT_CMAKE_MODULES)
