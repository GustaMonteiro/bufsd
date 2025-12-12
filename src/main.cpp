#include <iostream>

#include "bufsd/serializer.h"
#include "bufsd/serializable.h"
#include "bufsd/deserializable.h"
#include "bufsd/deserializer.h"
#include "bufsd/utils.h"

struct Person : public bufsd::Serializable, public bufsd::Deserializable<Person>
{
    std::string name;
    unsigned char age;

    std::vector<unsigned char> serialize() const
    {
        bufsd::Serializer serializer;

        serializer.push_16_big_endian(static_cast<unsigned short>(name.size()));
        serializer.push_buffer(std::vector<unsigned char>(name.begin(), name.end()));
        serializer.push_byte(age);

        return serializer.get_buffer();
    }

    void fill_from_bytes(bufsd::Deserializer &deserializer)
    {
        unsigned short name_length = deserializer.get_16_big_endian();
        std::vector<unsigned char> name_buffer = deserializer.get_buffer(name_length);
        this->name = std::string(name_buffer.begin(), name_buffer.end());
        this->age = deserializer.get_byte();
    }
};

int main()
{
    bufsd::Serializer serializer;

    serializer.push_16_big_endian(static_cast<unsigned short>(0x1234))
        .push_32_little_endian(0x56789abc)
        .defer_buffer_size_32_big_endian()
        .push_64_big_endian(0xdef0123456789abc);

    std::vector<unsigned char> buffer = serializer.get_buffer();

    std::cout << "Serialized Buffer: " << serializer.get_buffer_string() << std::endl;

    bufsd::Deserializer deserializer(buffer);
    unsigned short val16 = deserializer.get_16_big_endian();
    unsigned int val32 = deserializer.get_32_little_endian();
    deserializer.skip(4); // Skip the deferred size
    unsigned long long val64 = deserializer.get_64_big_endian();
    std::cout << "Deserialized Values: " << std::hex << val16 << ", " << val32 << ", " << val64 << std::dec << std::endl;

    Person person;
    person.name = "Alice";
    person.age = 30;

    std::vector<unsigned char> person_buffer = person.serialize();
    std::cout << "Serialized Person: " << person.to_string() << std::endl
              << "Person Name: " << person.name
              << ", Age: " << static_cast<int>(person.age) << std::endl;

    Person deserialized_person = Person::from_bytes(person_buffer);
    std::cout << "Deserialized Person Name: " << deserialized_person.name
              << ", Age: " << static_cast<int>(deserialized_person.age) << std::endl;

    std::vector<unsigned char> bytes = bufsd::hex_string_to_byte_vector("DE AD BE EF 00 11 22 33");
    std::cout << "Hex String to Byte Vector: ";
    for (unsigned char byte : bytes)
    {
        std::cout << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << std::endl;
}
