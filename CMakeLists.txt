cmake_minimum_required (VERSION 2.8)

project (LPROJ_main)

#setting necessary headers
include_directories (
	${PROJECT_SOURCE_DIR}/tools/json/src
	${PROJECT_SOURCE_DIR}/include
)

set(CMAKE_SKIP_RPATH FALSE)

add_subdirectory ( include )
add_subdirectory ( src )
add_subdirectory ( test )
add_subdirectory ( doc )

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

find_package( GCOV )
find_package( GCOVR )

add_custom_target(coverage
	WORKING_DIRECTORY test
	COMMAND libproject_test
	COMMAND ${GCOV} -o ../src/CMakeFiles/project.dir/libproject.cpp.gcno ../../src/libproject.cpp
	COMMAND ${CMAKE_COMMAND} -E make_directory ../coverage_report
	COMMAND ${GCOVR_EXECUTABLE} --object-directory= ../src -r ../../src/ --html --html-details -o ../coverage_report/report.html
	)

add_dependencies(coverage libproject_test)

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)