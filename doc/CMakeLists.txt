cmake_minimum_required (VERSION 2.8)

#configuring `documentation` target
if(DOXYGEN_FOUND)
        set(DOXY_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../src)
        configure_file(Doxyfile.in doc/Doxyfile)
        add_custom_target(documentation
        		ALL
                ${DOXYGEN_EXECUTABLE} doc/Doxyfile
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating API documentation with Doxygen" VERBATIM
                SOURCES Doxyfile.in
                )

		#part of configuring `install` target (documentation)
        install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html_doc/
        		DESTINATION share/doc/libproject
        )
endif(DOXYGEN_FOUND)

