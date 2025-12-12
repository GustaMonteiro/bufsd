# bufsd - Buffer Serialization/Deserialization Library

A modern C++17 header-mostly library for efficient binary data serialization and deserialization with support for big-endian and little-endian byte ordering.

## Features

- **Type-safe serialization/deserialization** - Template-based API with compile-time type checking
- **Endianness control** - Explicit big-endian and little-endian support for cross-platform compatibility
- **Fluent interface** - Chainable method calls for clean, readable code
- **Custom object serialization** - Simple interfaces for serializing complex types
- **Deferred buffer size** - Built-in support for writing length prefixes
- **Zero dependencies** - Uses only the C++ standard library
- **Header-mostly design** - Easy integration into existing projects

## Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.20 or higher

## Installation

### Using CMake

1. Clone the repository:
```bash
git clone https://github.com/GustaMonteiro/bufsd.git
cd bufsd
```

2. Build and install:
```bash
mkdir build && cd build
cmake ..
cmake --build .
sudo cmake --install .
```

### As a Submodule

Add bufsd as a git submodule to your project:
```bash
git submodule add https://github.com/GustaMonteiro/bufsd.git external/bufsd
```

Then in your CMakeLists.txt:
```cmake
add_subdirectory(external/bufsd)
target_link_libraries(your_target PRIVATE bufsd::bufsd)
```

## Quick Start

### Basic Serialization

```cpp
#include <bufsd/serializer.h>
#include <bufsd/deserializer.h>

// Create a serializer
bufsd::Serializer serializer;

// Add values with explicit endianness
serializer.push_16_big_endian(static_cast<unsigned short>(0x1234))
          .push_32_little_endian(0x56789abc)
          .push_64_big_endian(0xdef0123456789abc);

// Get the serialized buffer
std::vector<unsigned char> buffer = serializer.get_buffer();

// Deserialize
bufsd::Deserializer deserializer(buffer);
unsigned short val16 = deserializer.get_16_big_endian();
unsigned int val32 = deserializer.get_32_little_endian();
unsigned long long val64 = deserializer.get_64_big_endian();
```

### Custom Object Serialization

```cpp
#include <bufsd/serializable.h>
#include <bufsd/deserializable.h>
#include <bufsd/serializer.h>
#include <bufsd/deserializer.h>

struct Person : public bufsd::Serializable, public bufsd::Deserializable<Person>
{
    std::string name;
    unsigned char age;

    // Implement serialization
    std::vector<unsigned char> serialize() const override
    {
        bufsd::Serializer serializer;
        
        serializer.push_16_big_endian(static_cast<unsigned short>(name.size()));
        serializer.push_buffer(std::vector<unsigned char>(name.begin(), name.end()));
        serializer.push_byte(age);
        
        return serializer.get_buffer();
    }

    // Implement deserialization
    void fill_from_bytes(bufsd::Deserializer &deserializer) override
    {
        unsigned short name_length = deserializer.get_16_big_endian();
        std::vector<unsigned char> name_buffer = deserializer.get_buffer(name_length);
        this->name = std::string(name_buffer.begin(), name_buffer.end());
        this->age = deserializer.get_byte();
    }
};

// Usage
Person person{"Alice", 30};
std::vector<unsigned char> buffer = person.serialize();

Serializer serializer;
serializer.push_object(person); // uses .serialize() internally

// Deserialize
Person deserialized = Person::from_bytes(buffer);
```

## API Reference

### Serializer

#### Construction
```cpp
bufsd::Serializer()                      // Default constructor with 1024 bytes reserved
bufsd::Serializer(size_t size, unsigned char value = 0)  // Pre-allocated buffer
```

#### Serialization Methods

**Big-Endian (Network Byte Order)**
```cpp
Serializer& push_16_big_endian(T value)      // 2 bytes
Serializer& push_32_big_endian(T value)      // 4 bytes
Serializer& push_64_big_endian(T value)      // 8 bytes
Serializer& push_big_endian(T value)         // sizeof(T) bytes
```

**Little-Endian**
```cpp
Serializer& push_16_little_endian(T value)   // 2 bytes
Serializer& push_32_little_endian(T value)   // 4 bytes
Serializer& push_64_little_endian(T value)   // 8 bytes
Serializer& push_little_endian(T value)      // sizeof(T) bytes
```

**Buffer Operations**
```cpp
Serializer& push_byte(unsigned char byte)                    // Single byte
Serializer& push_buffer(const std::vector<unsigned char>&)   // Raw buffer
Serializer& push_object(const Serializable& object)          // Serializable object
```

**Deferred Size**
```cpp
Serializer& defer_buffer_size_16_big_endian()    // Reserve 2 bytes for size
Serializer& defer_buffer_size_32_big_endian()    // Reserve 4 bytes for size
Serializer& defer_buffer_size_64_big_endian()    // Reserve 8 bytes for size
// Similar methods for little-endian variants
```

**Retrieval**
```cpp
std::vector<unsigned char> get_buffer() const    // Get serialized buffer
std::string get_buffer_string() const            // Hex string representation
```

### Deserializer

#### Construction
```cpp
bufsd::Deserializer(const std::vector<unsigned char>& buffer)
```

#### Deserialization Methods

**Big-Endian**
```cpp
unsigned short get_16_big_endian()        // Read 2 bytes
unsigned int get_32_big_endian()          // Read 4 bytes
unsigned long long get_64_big_endian()    // Read 8 bytes
```

**Little-Endian**
```cpp
unsigned short get_16_little_endian()     // Read 2 bytes
unsigned int get_32_little_endian()       // Read 4 bytes
unsigned long long get_64_little_endian() // Read 8 bytes
```

**Buffer Operations**
```cpp
unsigned char get_byte()                           // Read single byte
std::vector<unsigned char> get_buffer(size_t size) // Read n bytes
void skip(size_t amount_of_bytes)                  // Skip bytes
```

**State**
```cpp
size_t get_cursor() const         // Current position
size_t get_remaining() const      // Bytes remaining
size_t get_buffer_size() const    // Total buffer size
```

### Interfaces

#### Serializable
```cpp
struct Serializable {
    virtual std::vector<unsigned char> serialize() const = 0;
    std::string to_string() const;  // Hex representation
};
```

#### Deserializable<T>
```cpp
template <typename T>
struct Deserializable {
    virtual void fill_from_bytes(Deserializer& deserializer) = 0;
    void fill_from_bytes(const std::vector<unsigned char>& buffer);
    static T from_bytes(const std::vector<unsigned char>& buffer);
    static T from_bytes(Deserializer& deserializer);
};
```

### Utility Functions

```cpp
// Convert buffer to hex string representation
std::string make_buffer_string(const std::vector<unsigned char>& buffer);

// Convert hex string to byte vector (e.g., "DE AD BE EF" -> {0xDE, 0xAD, 0xBE, 0xEF})
std::vector<unsigned char> hex_string_to_byte_vector(const std::string& hex_string);
```

## Advanced Features

### Deferred Buffer Size

When you need to write a length-prefixed buffer but don't know the size upfront:

```cpp
bufsd::Serializer serializer;

// Reserve space for a 4-byte size field
serializer.defer_buffer_size_32_big_endian();

// Add data (the library tracks the size)
serializer.push_64_big_endian(0x1234567890abcdef);
serializer.push_buffer(some_data);

// The deferred size is automatically filled with the correct value
```

### Nested Objects

```cpp
struct Inner : public bufsd::Serializable {
    int value;
    std::vector<unsigned char> serialize() const override {
        bufsd::Serializer s;
        s.push_32_big_endian(value);
        return s.get_buffer();
    }
};

struct Outer : public bufsd::Serializable {
    Inner inner;
    
    std::vector<unsigned char> serialize() const override {
        bufsd::Serializer s;
        s.push_object(inner);  // Serialize nested object
        return s.get_buffer();
    }
};
```

## Building Examples

To build and run the included examples:

```bash
mkdir build && cd build
cmake .. -DBUFSD_BUILD_EXAMPLES=ON
cmake --build .
./examples/basic_example
```

## Error Handling

The library throws `std::runtime_error` in the following cases:
- Attempting to read beyond buffer boundaries
- Type size mismatch (e.g., passing a 4-byte int to `push_16_big_endian`)
- Invalid hex string format in utility functions

## Performance Considerations

- **Buffer reservation**: The `Serializer` reserves 1024 bytes by default to minimize reallocations
- **In-place deserialization**: Use `fill_from_bytes` to reuse existing objects

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.
