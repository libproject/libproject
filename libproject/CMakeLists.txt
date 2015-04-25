cmake_minimum_required (VERSION 2.8)

project (libproject_main)

set(CMAKE_SKIP_RPATH FALSE)

add_subdirectory ( src )
add_subdirectory ( test )

add_custom_target(coverage
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test
	COMMAND libproject_test
	COMMAND gcov -o ../src/CMakeFiles/project.dir/libproject.cpp.gcno ../../src/libproject.cpp
	COMMAND mkdir ../coverage_report
	COMMAND gcovr --object-directory= ../src -r ../../src/ --html --html-details -o ../coverage_report/report.html
	)

add_dependencies(coverage libproject_test)
