cmake_minimum_required(VERSION 3.20)
project(bufsd VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

# Option to build examples
option(BUFSD_BUILD_EXAMPLES "Build example programs" OFF)

# Library sources (only implementation files, excluding main.cpp)
file(GLOB_RECURSE lib_src_files CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(FILTER lib_src_files EXCLUDE REGEX ".*main\\.cpp$")

# Create library
add_library(${PROJECT_NAME} STATIC ${lib_src_files})

# Include directories
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Build examples if option is enabled
if(BUFSD_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
