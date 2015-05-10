cmake_minimum_required (VERSION 2.8)

#setting name
project (libproject)

#setting sources
set (SOURCES
	libproject.cpp)

#setting what to build
add_library(project SHARED ${SOURCES})

#part of configuring `install` target (library)
install(TARGETS project
	LIBRARY
	DESTINATION lib
	)

#setting flags
set (CMAKE_CXX_FLAGS
	"${CMAKE_CXX_FLAGS} -std=c++11 -g -O0 -fprofile-arcs -ftest-coverage")
set (CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} -std=c++11 -g -O0 -fprofile-arcs -ftest-coverage")
set (CMAKE_EXE_LINKER_FLAGS
	"${CMAKE_EXE_LINKER_FLAGS} -std=c++11 -g -O0 -fprofile-arcs -ftest-coverage")

