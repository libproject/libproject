cmake_minimum_required (VERSION 2.8)

enable_testing ()

set(CMAKE_SKIP_RPATH FALSE)

add_subdirectory ( src )
add_subdirectory ( test )

add_custom_target(coverage
	COMMAND sh ${CMAKE_SOURCE_DIR}/do_coverage.sh
	)

#add_dependencies (coverage src)
