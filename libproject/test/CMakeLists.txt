cmake_minimum_required (VERSION 2.8)

project (libproject_test)

set (GMOCK_DIR
	"../../../gmock")

set (SOURCES
	../src/libproject.cpp
	libprojecttest.cpp
	${GMOCK_DIR}/src/gmock-all.cc
	${GMOCK_DIR}/gtest/src/gtest-all.cc)

include_directories (../../tools/json/src/
	../src
	${GMOCK_DIR}/gtest/include
	${GMOCK_DIR}/include
	${GMOCK_DIR}/gtest
	${GMOCK_DIR})

add_executable (libproject_test ${SOURCES})

execute_process (
	COMMAND cp ${CMAKE_SOURCE_DIR}/project_files ${CMAKE_BINARY_DIR} -R
	)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -O0")
