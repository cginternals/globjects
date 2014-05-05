# Generate meta-header
configure_file(glow-version.h.in ${CMAKE_CURRENT_BINARY_DIR}/glow-version.h)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Code Generation
add_subdirectory(codegeneration)

# Libraries
set(IDE_FOLDER "")
add_subdirectory(glow)
add_subdirectory(glowbase)
add_subdirectory(glowutils)
add_subdirectory(glowwindow)

# Examples
set(IDE_FOLDER "Examples")
add_subdirectory(examples)

# Tests
set(IDE_FOLDER "Tests")
add_subdirectory(tests)
