cmake_minimum_required (VERSION 2.8)

file(GLOB FILES_LIST "${CMAKE_CURRENT_SOURCE_DIR}/libproject/*.h")

install(FILES ${FILES_LIST}
        DESTINATION include/libproject
        )