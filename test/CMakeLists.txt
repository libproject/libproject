cmake_minimum_required (VERSION 2.8)

#cmake_policy(SET CMP0015 OLD)

#setting name
project (libproject_test)

#setting sources
set (GMOCK_DIR
	"../../gmock")
set (SOURCES_TEST
	libprojecttest.cpp
	${GMOCK_DIR}/src/gmock-all.cc
	${GMOCK_DIR}/gtest/src/gtest-all.cc)

#setting necessary headers
include_directories (../tools/json/src/
	../src
	${GMOCK_DIR}/gtest/include
	${GMOCK_DIR}/include
	${GMOCK_DIR}/gtest
	${GMOCK_DIR})

#setting what to build
add_executable (libproject_test ${SOURCES_TEST})

#searching for shared library
target_link_libraries (libproject_test project)

#preparing test environments
execute_process (
	COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/project_files ${CMAKE_CURRENT_BINARY_DIR} -R
	)

#setting flags
set (CMAKE_CXX_FLAGS
	"${CMAKE_CXX_FLAGS} -std=c++11 -pthread -g -O0")
set (CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} -std=c++11 -pthread -g -O0")
set (CMAKE_EXE_LINKER_FLAGS
	"${CMAKE_EXE_LINKER_FLAGS} -std=c++11 -pthread -g -O0")
