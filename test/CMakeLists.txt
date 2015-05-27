cmake_minimum_required (VERSION 2.8)

#setting name
project (libproject_test)

#setting sources
set (SOURCES_TEST
	libprojecttest.cpp
)

#setup tests
find_package(GMock)

if (GMOCK_FOUND)
    include_gmock()

    #setting necessary headers
    include_directories (${GMOCK_INCLUDE_DIRS})

    #setting what to build
    add_executable (libproject_test ${SOURCES_TEST})

    #searching for shared library
    target_link_libraries (libproject_test project ${GMOCK_LIBRARIES})

    #preparing test environments
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/project_files DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

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
            COMMAND ${GCOV_EXECUTABLE} -o ../src/CMakeFiles/project.dir/libproject.cpp.gcno ../../src/libproject.cpp
            COMMAND ${CMAKE_COMMAND} -E make_directory ../coverage_report
            COMMAND ${GCOVR_EXECUTABLE} --object-directory= ../src -r ${CMAKE_SOURCE_DIR}/src/ --html --html-details -o ../coverage_report/report.html
            )
        add_dependencies(coverage run_tests)
    endif(GCOV_FOUND AND GCOVR_FOUND)

else (GMOCK_FOUND)
    message("GMock wasn't found, tests and corresponding targest will be unavailable")
endif(GMOCK_FOUND)




