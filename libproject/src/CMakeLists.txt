cmake_minimum_required (VERSION 2.8)

project (libproject)

set (SOURCES
	libproject.cpp)

include_directories (../../tools/json/src/)

add_library(project SHARED ${SOURCES})

set(CMAKE_CXX_FLAGS "-std=c++11")
