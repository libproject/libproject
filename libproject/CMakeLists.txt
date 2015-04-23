cmake_minimum_required (VERSION 2.8)

enable_testing ()

set(CMAKE_SKIP_RPATH FALSE)

add_subdirectory ( src )
add_subdirectory ( test )

add_custom_target(coverage
		COMMAND touch ${CMAKE_BINARY_DIR}/test
		# ...
	)
