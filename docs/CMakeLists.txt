
set(doxyfile doxyfile.in)

set(CMAKETOOLS_CURRENT_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

# Set project variables
set(CMAKETOOLS_CURRENT_TARGET_DOXYFILE ${doxyfile})
set(CMAKETOOLS_CURRENT_TARGET_HHP  ${CMAKETOOLS_CURRENT_OUTPUT_DIR}/html/index.hhp)
set(CMAKETOOLS_CURRENT_TARGET_DOXYFILE_CONF ${CMAKETOOLS_CURRENT_OUTPUT_DIR}/doxyfile)

find_package(Doxygen)

set(target glow)

# Check if doxygen is available
if(DOXYGEN_FOUND)

    # Get filename and path of doxyfile
    get_filename_component(name ${doxyfile} NAME)
    get_filename_component(path ${doxyfile} PATH)
    if(NOT path)
        set(path ${CMAKE_CURRENT_SOURCE_DIR})
    endif()

    # Configure doxyfile (if it is a real doxyfile already, it should simply copy the file)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKETOOLS_CURRENT_OUTPUT_DIR})
    configure_file(${CMAKETOOLS_CURRENT_TARGET_DOXYFILE} ${CMAKETOOLS_CURRENT_TARGET_DOXYFILE_CONF})

    # Invoke doxygen
    add_custom_command(
        OUTPUT              ${CMAKETOOLS_CURRENT_TARGET_HHP}
        DEPENDS             ${CMAKETOOLS_CURRENT_TARGET_DOXYFILE_CONF}
        WORKING_DIRECTORY   ${path}
        COMMAND             ${CMAKE_COMMAND} -E copy_directory ${path} ${CMAKETOOLS_CURRENT_OUTPUT_DIR}/html
        COMMAND             ${DOXYGEN} \"${CMAKETOOLS_CURRENT_TARGET_DOXYFILE_CONF}\"
	COMMAND             ${CMAKE_COMMAND} -E copy_directory ${path}/extra_files ${CMAKETOOLS_CURRENT_OUTPUT_DIR}/html
        COMMENT             "Creating doxygen documentation for ${target}"
    )

    # Declare a target
    add_custom_target(docs-doxygen
        DEPENDS ${CMAKETOOLS_CURRENT_TARGET_HHP}
    )
endif()
