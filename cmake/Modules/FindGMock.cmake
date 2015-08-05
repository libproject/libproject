# Finds GMock source and provide function which includes them as subproject
# Input:
# GMOCK_ROOT - should point to gmock source package root, can be setted as environment variable
# environment variable will be preffered. If no one is set also CMAKE_PREFIX_PATH will be used and
#  ../gmock path revieved
#
# Provides:
# GMOCK_FOUND - setted true if GMock was found
# include_gmock() function - includes GMock source as CMake's subproject
# GMOCK_INCLUDE_DIRS - include directories
# GMOCK_LIBRARIES - libraries which should be linked to create gmock based tests
# GMOCK_MAIN_LIBRARIES - libraries containing default gmock initialized int main()
# GMOCK_BOTH_LIBRARIES - both GMOCK_LIBRARIES and GMOCK_MAIN_LIBRARIES
# GTEST_ADD_TEST () function - borrowed from kitware's FindGTest.cmake,
# watch documentation at the end of documentation section
#
# Usage sample:
#
# set(GMOCK_SOURCE <some path>)
# find_package(GMock REQUIRED)
# if (GMOCK_FOUND)
#  include_gmock()
# endif(GMOCK_FOUND)
# target_link_libraries(<some_target> ${GMOCK_BOTH_LIBRARIES})
#
# If you would like each Google test to show up in CTest as a test you
# may use the following macro.  NOTE: It will slow down your tests by
# running an executable for each test and test fixture.  You will also
# have to rerun CMake after adding or removing tests or test fixtures.
#
# GTEST_ADD_TESTS(executable extra_args ARGN)
#
# ::
#
#     executable = The path to the test executable
#     extra_args = Pass a list of extra arguments to be passed to
#                  executable enclosed in quotes (or "" for none)
#     ARGN =       A list of source files to search for tests & test
#                  fixtures.
#
#
#
# ::
#
#   Example:
#      set(FooTestArgs --foo 1 --bar 2)
#      add_executable(FooTest FooUnitTest.cc)
#      GTEST_ADD_TESTS(FooTest "${FooTestArgs}" FooUnitTest.cc)
#=============================================================================
# Copyright 2009 Kitware, Inc.
# Copyright 2009 Philip Lowman <philip@yhbt.com>
# Copyright 2009 Daniel Blezek <blezek@gmail.com>
# Copyright 2015 Maxim Kot <work.maydjin@gmail.com>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)
#
# Thanks to Daniel Blezek <blezek@gmail.com> for the GTEST_ADD_TESTS code

function(GTEST_ADD_TESTS executable extra_args)
    if(NOT ARGN)
        message(FATAL_ERROR "Missing ARGN: Read the documentation for GTEST_ADD_TESTS")
    endif()
    foreach(source ${ARGN})
        file(READ "${source}" contents)
        string(REGEX MATCHALL "TEST_?F?\\(([A-Za-z_0-9 ,]+)\\)" found_tests ${contents})
        foreach(hit ${found_tests})
            string(REGEX REPLACE ".*\\( *([A-Za-z_0-9]+), *([A-Za-z_0-9]+) *\\).*" "\\1.\\2" test_name ${hit})
            add_test(${test_name} ${executable} --gtest_filter=${test_name} ${extra_args})
        endforeach()
    endforeach()
endfunction()

set (CMAKE_ROOT "" CACHE PATH "Path to GMock source directory")

if (NOT $ENV{GMOCK_ROOT} STREQUAL "")
    message (FATAL "ENV")
    set (GMOCK_ROOT $ENV{GMOCK_ROOT})
endif (NOT $ENV{GMOCK_ROOT} STREQUAL "")

set (gmock_search_path ${GMOCK_ROOT})

if (NOT GMOCK_ROOT)
    message("GMOCK_ROOT isn't set, so we'll try to find GMock anywhere...")
    set (gmock_search_path "<Anywhere>")
endif(NOT GMOCK_ROOT)


message (STATUS "Searching GMock source at: '${gmock_search_path}'...")

find_path(GMOCK_SOURCE src/gmock-all.cc
    HINTS
        ${GMOCK_ROOT}
        "${CMAKE_SOURCE_DIR}/../gmock"
)

message (STATUS "Gmock source is ${GMOCK_SOURCE}")

if (GMOCK_SOURCE)
    set (GMOCK_INCLUDE_DIRS ${GMOCK_SOURCE}/include/ ${GMOCK_SOURCE}/gtest/include)
    set (GMOCK_LIBRARIES gmock)
    set (GMOCK_MAIN_LIBRARIES gmock_main)
    set (GMOCK_BOTH_LIBRARIES ${GMOCK_LIBRARIES} ${GMOCK_BOTH_LIBRARIES})
endif (GMOCK_SOURCE)

function(include_gmock)
         add_subdirectory(${GMOCK_SOURCE} ${CMAKE_BINARY_DIR}/gmock)
endfunction()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GMock DEFAULT_MSG
GMOCK_SOURCE
GMOCK_INCLUDE_DIRS
GMOCK_LIBRARIES
GMOCK_MAIN_LIBRARIES
)
