
# Target
set(target glowbase)
message(STATUS "Lib ${target}")

#
# External libraries
#

#
# Includes
#

include_directories(
)

include_directories(
    BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#
# Libraries
#

set(libs
)

#
# Compiler definitions
#

if (OPTION_BUILD_STATIC)
    add_definitions("-DGLOWBASE_STATIC")
else()
    add_definitions("-DGLOWBASE_EXPORTS")
endif()

#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(sources
	${source_path}/AbstractFunctionCall.cpp
	${source_path}/AbstractStringSource.cpp
	${source_path}/baselogging.cpp
	${source_path}/Changeable.cpp
	${source_path}/ChangeListener.cpp
	${source_path}/CompositeStringSource.cpp
	${source_path}/ConsoleLogger.cpp
	${source_path}/File.cpp
	${source_path}/FileRegistry.cpp
	${source_path}/FileRegistry.h
	${source_path}/formatString.cpp
	${source_path}/HeapOnly.cpp
	${source_path}/LogMessageBuilder.cpp
	${source_path}/LogMessage.cpp
	${source_path}/Referenced.cpp
	${source_path}/StaticStringSource.cpp
	${source_path}/Version.cpp
)

set(api_includes
	${include_path}/AbstractStringSource.h
	${include_path}/AbstractFunctionCall.h
	${include_path}/AbstractLogHandler.h
	${include_path}/baselogging.h
	${include_path}/baselogging.hpp
	${include_path}/CachedValue.h
	${include_path}/CachedValue.hpp
	${include_path}/Changeable.h
	${include_path}/ChangeListener.h
	${include_path}/CompositeStringSource.h
	${include_path}/ConsoleLogger.h
	${include_path}/container_helpers.hpp
	${include_path}/File.h
	${include_path}/formatString.h
	${include_path}/formatString.hpp
	${include_path}/FunctionCall.h
	${include_path}/FunctionCall.hpp
	${include_path}/glowbase_api.h
	${include_path}/HeapOnly.h
	${include_path}/LogMessageBuilder.h
	${include_path}/LogMessageBuilder.hpp
	${include_path}/LogMessage.h
	${include_path}/Referenced.h
	${include_path}/ref_ptr.h
	${include_path}/ref_ptr.hpp
	${include_path}/StaticStringSource.h
	${include_path}/Version.h
)

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.hpp$" 
    ${header_group} ${api_includes})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${source_group} ${sources})

#
# Build library
#

if(OPTION_BUILD_STATIC)
    add_library(${target} ${api_includes} ${sources})
else()
    add_library(${target} SHARED ${api_includes} ${sources})
endif()
target_link_libraries(${target} ${libs})

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE              CXX
    FOLDER                      "${IDE_FOLDER}"
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_FLAGS               "${DEFAULT_COMPILE_FLAGS}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}"
    INCLUDE_PATH	            ${include_path})

#
# Deployment
#

# Library
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}
    LIBRARY DESTINATION ${INSTALL_SHARED}
    ARCHIVE DESTINATION ${INSTALL_LIB}
)

# Header files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE})
