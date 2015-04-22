cmake_minimum_required (VERSION 2.8)

project (libproject_test)

set (GMOCK_DIR
	"../../../gmock")

set (SOURCES_TEST
#	../src/libproject.cpp
	libprojecttest.cpp
	${GMOCK_DIR}/src/gmock-all.cc
	${GMOCK_DIR}/gtest/src/gtest-all.cc)

set (SOURCES_LIB
	../src/libproject.cpp)


include_directories (../../tools/json/src/
	../src
	${GMOCK_DIR}/gtest/include
	${GMOCK_DIR}/include
	${GMOCK_DIR}/gtest
	${GMOCK_DIR})

add_library (project SHARED ${SOURCES_LIB})

add_executable (libproject_test ${SOURCES_TEST})

target_link_libraries(libproject_test project)

execute_process (
	COMMAND cp ${CMAKE_SOURCE_DIR}/project_files ${CMAKE_BINARY_DIR} -R
	)

set (CMAKE_CXX_FLAGS
	"${CMAKE_CXX_FLAGS} -std=c++11 -pthread -O0 -fprofile-arcs -ftest-coverage -fprofile-dir=${CMAKE_BINARY_DIR}")

set (CMAKE_EXE_LINKER_FLAGS
	"${CMAKE_EXE_LINKER_FLAGS} -std=c++11 -pthread -O0 -fprofile-arcs -ftest-coverage -fprofile-dir=${CMAKE_BINARY_DIR}")
