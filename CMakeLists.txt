cmake_minimum_required(VERSION 3.22)
project(mge VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

option(MGE_BUILD_EXAMPLES "Build examples" OFF)

if(WIN32)
    add_compile_definitions(_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS)
endif()

set(BUILD_ROOT ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)
add_subdirectory(external)
if(MGE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()