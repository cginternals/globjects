
# support for C++11 etc.

set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -stdlib=libc++ -g -Wall")

    
set(MACOS_COMPILE_DEFS
)
set(DEFAULT_COMPILE_DEFS_DEBUG
    ${MACOS_COMPILE_DEFS}
    _DEBUG                      # Debug build
)
set(DEFAULT_COMPILE_DEFS_RELEASE
    ${MACOS_COMPILE_DEFS}
    NDEBUG                      # Release build
)


set(MACOS_COMPILE_FLAGS
)
set(DEFAULT_COMPILE_FLAGS ${MACOS_COMPILE_FLAGS})

