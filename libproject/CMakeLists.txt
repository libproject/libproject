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


find_package(Doxygen)
if(DOXYGEN_FOUND)
	set(DOXY_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile)
	add_custom_target(documentation
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen" VERBATIM
		)

endif(DOXYGEN_FOUND)
