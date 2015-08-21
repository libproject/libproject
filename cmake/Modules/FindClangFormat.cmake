INCLUDE(FindPackageHandleStandardArgs)
FIND_PROGRAM(CLANGFORMAT_EXECUTABLE clang-format HINTS ${CLANGFORMAT_ROOT})
FIND_PACKAGE_HANDLE_STANDARD_ARGS(clang-format DEFAULT_MSG CLANGFORMAT_EXECUTABLE)
# only visible in advanced view
MARK_AS_ADVANCED(CLANGFORMAT_EXECUTABLE)

if(NOT CLANGFORMAT_EXECUTABLE)
 set(CLANGFORMAT_FOUND FALSE)
 message(STATUS "clang-format not found! You can't perform codestyle normalizing by make target...")
elseif()
 set(CLANGFORMAT_FOUND TRUE)
endif()
