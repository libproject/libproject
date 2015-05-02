cmake_minimum_required (VERSION 2.8)

project (LPROJ_main)

set(CMAKE_SKIP_RPATH FALSE)

add_subdirectory ( libproject/src )
add_subdirectory ( libproject/test )

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

find_package( GCOV )
find_package( GCOVR )

add_custom_target(coverage
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/libproject/test
	COMMAND libproject_test
	COMMAND ${GCOV} -o ../src/CMakeFiles/project.dir/libproject.cpp.gcno ../../../libproject/src/libproject.cpp
	COMMAND ${CMAKE_COMMAND} -E make_directory ../coverage_report
	COMMAND ${GCOVR_EXECUTABLE} --object-directory= ../src -r ../../../libproject/src/ --html --html-details -o ../coverage_report/report.html
	)
add_dependencies(coverage libproject_test)

find_package(Doxygen)
if(DOXYGEN_FOUND)
	set(DOXY_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libproject/src)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libproject/doc/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile)
	add_custom_target(documentation
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen" VERBATIM
		SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/libproject/doc/Doxyfile.in
		)
elseif()
	MESSAGE(FATAL_ERROR "Doxygen not found! Aborting...")
endif(DOXYGEN_FOUND)

