# Examples for bufsd library

# Basic example demonstrating serialization and deserialization
add_executable(basic_example basic_example.cpp)
target_link_libraries(basic_example PRIVATE bufsd::bufsd)
