message(STATUS "Configuring for platform Linux/GCC.")


# Enable C++11 support

execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
	OUTPUT_VARIABLE GCC_VERSION)

if(GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
elseif(GCC_VERSION VERSION_EQUAL 4.6)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()


set(LINUX_COMPILE_DEFS
	LINUX	                  # Linux system
        PIC		          # Position-independent code
	_REENTRANT                # Reentrant code
)
set(DEFAULT_COMPILE_DEFS_DEBUG
    ${LINUX_COMPILE_DEFS}
    _DEBUG	                  # Debug build
)
set(DEFAULT_COMPILE_DEFS_RELEASE
    ${LINUX_COMPILE_DEFS}
    NDEBUG	                  # Release build
)

if (OPTION_ERRORS_AS_EXCEPTION)
	set(EXCEPTION_FLAG "-fexceptions")
else()
	set(EXCEPTION_FLAG "-fno-exceptions")
endif()

set(LINUX_COMPILE_FLAGS
      
      ${EXCEPTION_FLAG}
      -pthread      # -> use pthread library
    # -no-rtti      # -> disable c++ rtti
      -pipe         # -> use pipes
      -pedantic     # ->
      -Wall         # -> 
      -Wextra       # -> 
      -Werror       # ->
    # -pedantic     # -> yeah, be really pedantic
	
      -Wfloat-equal 
      -Wcast-qual 
      -Wcast-align 
      -Wconversion 
      -Wreturn-type 

      -Wno-error=float-equal 
      -Wno-error=conversion 
      -Wno-error=switch
      -Wno-error=comment
      -Wno-error=strict-aliasing

      -fPIC         # -> use position independent code
      -fvisibility=hidden # ->
    # -Werror=return-type -> missing returns in functions and methods are handled as errors which stops the compilation
    # -Wshadow      # -> e.g. when a parameter is named like a member, too many warnings, disabled for now
)

if (CMAKE_COMPILER_IS_GNUCXX)
    if (NOT (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "4.9"))
        set(LINUX_COMPILE_FLAGS ${LINUX_COMPILE_FLAGS}
            -Wno-error=float-conversion
        )
    endif ()
endif ()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	# clang
    set(LINUX_COMPILE_FLAGS ${LINUX_COMPILE_FLAGS}
      -Wno-mismatched-tags 
      -Wno-unsequenced 
      -Wno-sign-conversion 
      -Wno-unused-function 
      -Wno-missing-braces 
      -Wno-error=shorten-64-to-32
	)
endif()

set(DEFAULT_COMPILE_FLAGS
    ${LINUX_COMPILE_FLAGS}
    $<$<CONFIG:Debug>:   
    >
    $<$<CONFIG:Release>: 
    >
)

set(LINUX_LINKER_FLAGS "-pthread")

set(DEFAULT_LINKER_FLAGS_RELEASE ${LINUX_LINKER_FLAGS})
set(DEFAULT_LINKER_FLAGS_DEBUG ${LINUX_LINKER_FLAGS})
set(DEFAULT_LINKER_FLAGS ${LINUX_LINKER_FLAGS})

# Add platform specific libraries for linking
set(EXTRA_LIBS "")
