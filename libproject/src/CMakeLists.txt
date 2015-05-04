cmake_minimum_required (VERSION 2.8)

#setting name
project (libproject)

#setting sources
set (SOURCES
	libproject.cpp)

#setting necessary headers
include_directories (../../tools/json/src/)

#setting what to build
add_library(project SHARED ${SOURCES})

#configuring `install` target
install(TARGETS project
	LIBRARY
	DESTINATION lib
	)

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/libproject.h
        DESTINATION include
        )

#setting flags
set (CMAKE_CXX_FLAGS
	"${CMAKE_CXX_FLAGS} -std=c++11 -g -O0 -fprofile-arcs -ftest-coverage")
set (CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} -std=c++11 -g -O0 -fprofile-arcs -ftest-coverage")
set (CMAKE_EXE_LINKER_FLAGS
	"${CMAKE_EXE_LINKER_FLAGS} -std=c++11 -g -O0 -fprofile-arcs -ftest-coverage")

#configuring `documentation` target
find_package(Doxygen)
if(DOXYGEN_FOUND)
        set(DOXY_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR})
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../doc/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile)
        add_custom_target(documentation
                ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating API documentation with Doxygen" VERBATIM
                SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../doc/Doxyfile.in
                )
elseif()
        MESSAGE(FATAL_ERROR "Doxygen not found! Aborting...")
endif(DOXYGEN_FOUND)

