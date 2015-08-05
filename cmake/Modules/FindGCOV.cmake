
INCLUDE(FindPackageHandleStandardArgs)
FIND_PROGRAM(GCOV_EXECUTABLE gcov HINTS ${GCOV_ROOT})
FIND_PACKAGE_HANDLE_STANDARD_ARGS(gcov DEFAULT_MSG GCOV_EXECUTABLE)
# only visible in advanced view
MARK_AS_ADVANCED(GCOV_EXECUTABLE)

if(NOT GCOV_EXECUTABLE)
 set(GCOV_FOUND FALSE)
 message("gcov not found! You can't perform coverage tests...")
elseif(NOT GCOV_EXECUTABLE)
 set(GCOV_FOUND TRUE)
endif()
