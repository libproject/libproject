cmake_minimum_required (VERSION 2.8)

project (LPROJ_main)

#setting necessary headers
include_directories (
	${PROJECT_SOURCE_DIR}/tools/json/src
	${PROJECT_SOURCE_DIR}/include
)
#setting path to directory with cmake modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

set(CMAKE_SKIP_RPATH FALSE)
set(GCOV_FOUND FALSE)
set(GCOVR_FOUND FALSE)

#setting subprojects
add_subdirectory ( include )
add_subdirectory ( src )
add_subdirectory ( test )
add_subdirectory ( doc )



#configuring `uninstall` target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

