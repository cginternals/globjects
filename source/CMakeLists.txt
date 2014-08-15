# Generate meta-header
configure_file(globjects-version.h.in ${CMAKE_CURRENT_BINARY_DIR}/globjects-version.h)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Libraries
set(IDE_FOLDER "")
add_subdirectory(globjects)
add_subdirectory(globjects-base)
if (OPTION_BUILD_GLOBJECTS_UTILS)
	add_subdirectory(globjects-utils)
endif()
if (OPTION_BUILD_GLOBJECTS_WINDOW)
	add_subdirectory(globjects-window)
endif()

# Examples
set(IDE_FOLDER "Examples")
add_subdirectory(examples)

# Tests
set(IDE_FOLDER "Tests")
add_subdirectory(tests)
