cmake_minimum_required (VERSION 2.8)

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
include_directories (
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

#configuring `run_tests` target
add_custom_target(run_tests
	COMMAND libproject_test
	)
add_dependencies(run_tests libproject_test)

#configuring `coverage` target
find_package( GCOV )
find_package( GCOVR )
if(GCOV_FOUND AND GCOVR_FOUND)
	add_custom_target(coverage
#		WORKING_DIRECTORY test
		COMMAND ${GCOV_EXECUTABLE} -o ../src/CMakeFiles/project.dir/libproject.cpp.gcno ../../src/libproject.cpp
		COMMAND ${CMAKE_COMMAND} -E make_directory ../coverage_report
		COMMAND ${GCOVR_EXECUTABLE} --object-directory= ../src -r ../../src/ --html --html-details -o ../coverage_report/report.html
		)
	add_dependencies(coverage run_tests)
endif(GCOV_FOUND AND GCOVR_FOUND)