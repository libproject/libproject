cmake_minimum_required (VERSION 2.8)

enable_testing ()

add_subdirectory ( src EXCLUDE_FROM_ALL )
add_subdirectory ( test EXCLUDE_FROM_ALL )

add_custom_target(coverage
		COMMAND touch ${CMAKE_BINARY_DIR}/test
		# ...
	)
add_dependencies (test src)
#add_dependencies (coverage test)
