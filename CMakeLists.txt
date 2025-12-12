cmake_minimum_required(VERSION 3.20)
project(bufsd "CXX")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB_RECURSE src_files CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${src_files})
