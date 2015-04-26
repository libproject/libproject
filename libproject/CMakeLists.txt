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

add_custom_target(documentation
#	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/doc
#	COMMAND sed -e \'s%INPUT = %INPUT = ${CMAKE_SOURCE_DIR}/src/libproject/%g\' ${CMAKE_SOURCE_DIR}/doc/Doxyfile > Doxyfile
	configure_file(${CMAKE_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_BINARY_DIR}/doc/Doxyfile)
	COMMAND doxygen Doxyfile
	)

add_custom_command(TARGET documentation
	PRE_BUILD
	COMMAND mkdir ${CMAKE_BINARY_DIR}/doc
	)
