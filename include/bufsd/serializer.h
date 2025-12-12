#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <iomanip>

#include "bufsd/serializable.h"
#include "bufsd/utils.h"

namespace bufsd
{
	static std::string make_error_message(unsigned char expected, unsigned char received)
	{
		std::stringstream ss;
		ss << "Tried to push " << (int)expected << " bits value, but the input contains " << (int)received * 8 << " bits";
		return ss.str();
	}

	template <typename T>
	static void check_size(T value, unsigned char amount_of_bytes)
	{
		if (sizeof(T) != amount_of_bytes)
			throw std::runtime_error(make_error_message(amount_of_bytes * 8, sizeof(value)));
	}

	class Serializer
	{
	public:
		/// <summary>
		/// Constructs a new empty buffer maker.
		/// <para>The internal buffer reserves 1024 bytes to avoid possible reallocations.</para>
		/// </summary>
		Serializer()
		{
			this->buffer.reserve(1024);
		}

		/// <summary>
		/// Constructs a new buffer maker with <paramref name="size"/> initial space, with all bytes set to <paramref name="value"/>.
		/// </summary>
		/// <param name="size">Initial size</param>
		/// <param name="value">Value that all bytes will be set to</param>
		Serializer(size_t size, unsigned char value = 0)
		{
			this->buffer.resize(size, value);
		}

		/// <summary>
		/// Pushes <paramref name="value"/>'s bytes to the buffer in Little-Endian (inverting the order).
		/// <para>It accepts any numeric value, so if you use an int, 4 bytes will be pushed, if use unsigned short, 2 bytes will be pushed, and so on...</para>
		/// <example>If the number 32, as an int, be pushed, the bytes added will be { 0x20, 0x00, 0x00, 0x00 }.</example>
		/// </summary>
		/// <typeparam name="T">The type of the number, used to know how many bytes will be pushed</typeparam>
		/// <param name="value">Value to be pushed</param>
		/// <returns>Reference to the buffer maker object (allows chaining methods)</returns>
		template <typename T>
		Serializer &push_little_endian(T value)
		{
			for (int i = 0; i < sizeof(T); i++)
				this->buffer.push_back((value >> (i * 8)) & 0xff);
			return *this;
		}

		/// <summary>
		/// Pushes <paramref name="value"/>'s bytes to the buffer in Big-Endian (not inverting the order).
		/// <para>It accepts any numeric value, so if you use an int, 4 bytes will be pushed, if use unsigned short, 2 bytes will be pushed, and so on...</para>
		/// <example>If the number 32, as an int, be pushed, the bytes added will be { 0x00, 0x00, 0x00, 0x20 }.</example>
		/// </summary>
		/// <typeparam name="T">The type of the number, used to know how many bytes will be pushed</typeparam>
		/// <param name="value">Value to be pushed</param>
		/// <returns>Reference to the buffer maker object (allows chaining methods)</returns>
		template <typename T>
		Serializer &push_big_endian(T value)
		{
			for (int i = sizeof(T) - 1; i >= 0; i--)
				this->buffer.push_back((value >> (i * 8)) & 0xff);
			return *this;
		}

		/// <summary>
		/// Pushes <paramref name="value"/>'s bytes to the buffer in Big-Endian (not inverting the order).
		/// <para>It only accepts 2 bytes numeric values, so if use unsigned short, 2 bytes will be pushed.</para>
		/// <example>If the number 32, as an unsigned short, be pushed, the bytes added will be { 0x00, 0x20 }.</example>
		/// </summary>
		/// <typeparam name="T">The type of the number, used to know how many bytes will be pushed</typeparam>
		/// <param name="value">Value to be pushed</param>
		/// <returns>Reference to the buffer maker object (allows chaining methods)</returns>
		/// <exception cref="runtime_error">Occurs if the input types doesn't have 2 bytes</exception>
		template <typename T>
		Serializer &push_16_big_endian(T value)
		{
			check_size(value, 2);
			return this->push_big_endian(value);
		}

		/// <summary>
		/// Pushes <paramref name="value"/>'s bytes to the buffer in Big-Endian (not inverting the order).
		/// <para>It only accepts 4 bytes numeric values, so if use unsigned int, 4 bytes will be pushed.</para>
		/// <example>If the number 32, as an unsigned int, be pushed, the bytes added will be { 0x00, 0x00, 0x00, 0x20 }.</example>
		/// </summary>
		/// <typeparam name="T">The type of the number, used to know how many bytes will be pushed</typeparam>
		/// <param name="value">Value to be pushed</param>
		/// <returns>Reference to the buffer maker object (allows chaining methods)</returns>
		/// <exception cref="runtime_error">Occurs if the input types doesn't have 4 bytes</exception>
		template <typename T>
		Serializer &push_32_big_endian(T value)
		{
			check_size(value, 4);
			return this->push_big_endian(value);
		}

		/// <summary>
		/// Pushes <paramref name="value"/>'s bytes to the buffer in Big-Endian (not inverting the order).
		/// <para>It only accepts 8 bytes numeric values, so if use unsigned long long, 8 bytes will be pushed.</para>
		/// <example>If the number 32, as an unsigned long long, be pushed, the bytes added will be { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20 }.</example>
		/// </summary>
		/// <typeparam name="T">The type of the number, used to know how many bytes will be pushed</typeparam>
		/// <param name="value">Value to be pushed</param>
		/// <returns>Reference to the buffer maker object (allows chaining methods)</returns>
		/// <exception cref="runtime_error">Occurs if the input types doesn't have 8 bytes</exception>
		template <typename T>
		Serializer &push_64_big_endian(T value)
		{
			check_size(value, 8);
			return this->push_big_endian(value);
		}

		/// <summary>
		/// Pushes <paramref name="value"/>'s bytes to the buffer in Little-Endian (inverting the order).
		/// <para>It only accepts 2 bytes numeric values, so if use unsigned short, 2 bytes will be pushed.</para>
		/// <example>If the number 32, as an unsigned short, be pushed, the bytes added will be { 0x20, 0x00 }.</example>
		/// </summary>
		/// <typeparam name="T">The type of the number, used to know how many bytes will be pushed</typeparam>
		/// <param name="value">Value to be pushed</param>
		/// <returns>Reference to the buffer maker object (allows chaining methods)</returns>
		/// <exception cref="runtime_error">Occurs if the input types doesn't have 2 bytes</exception>
		template <typename T>
		Serializer &push_16_little_endian(T value)
		{
			check_size(value, 2);
			return this->push_little_endian(value);
		}

		/// <summary>
		/// Pushes <paramref name="value"/>'s bytes to the buffer in Little-Endian (inverting the order).
		/// <para>It only accepts 4 bytes numeric values, so if use unsigned int, 4 bytes will be pushed.</para>
		/// <example>If the number 32, as an unsigned int, be pushed, the bytes added will be { 0x20, 0x00, 0x00, 0x00 }.</example>
		/// </summary>
		/// <typeparam name="T">The type of the number, used to know how many bytes will be pushed</typeparam>
		/// <param name="value">Value to be pushed</param>
		/// <returns>Reference to the buffer maker object (allows chaining methods)</returns>
		/// <exception cref="runtime_error">Occurs if the input types doesn't have 4 bytes</exception>
		template <typename T>
		Serializer &push_32_little_endian(T value)
		{
			check_size(value, 4);
			return this->push_little_endian(value);
		}

		/// <summary>
		/// Pushes <paramref name="value"/>'s bytes to the buffer in Little-Endian (inverting the order).
		/// <para>It only accepts 8 bytes numeric values, so if use unsigned long long, 8 bytes will be pushed.</para>
		/// <example>If the number 32, as an unsigned long long, be pushed, the bytes added will be { 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }.</example>
		/// </summary>
		/// <typeparam name="T">The type of the number, used to know how many bytes will be pushed</typeparam>
		/// <param name="value">Value to be pushed</param>
		/// <returns>Reference to the buffer maker object (allows chaining methods)</returns>
		/// <exception cref="runtime_error">Occurs if the input types doesn't have 8 bytes</exception>
		template <typename T>
		Serializer &push_64_little_endian(T value)
		{
			check_size(value, 8);
			return this->push_little_endian(value);
		}

		/// <summary>
		/// Pushes <paramref name="values"/>'s bytes to the buffer, keeping the bytes order.
		/// </summary>
		/// <param name="values">Buffer to be pushed</param>
		/// <returns>Reference to the buffer maker object (allows chaining methods)</returns>
		Serializer &push_buffer(const std::vector<unsigned char> &values)
		{
			this->buffer.insert(this->buffer.end(), values.begin(), values.end());

			return *this;
		}

		/// <summary>
		/// Pushes any object that implements Serializable interface.
		/// <para>It calls the serialize() method and push the buffer.
		/// </summary>
		/// <param name="object">Object that implements Serializable interface</param>
		/// <returns>Reference to the buffer maker object (allows chaining methods)</returns>
		Serializer &push_object(const Serializable &object)
		{
			return this->push_buffer(object.serialize());
		}

		/// <summary>
		/// Pushes <paramref name="value"/> byte to the buffer.
		/// <para>It only accepts 1 byte numeric values, so if use unsigned char, 1 byte will be pushed.</para>
		/// <example>If the number 32, as an unsigned char, be pushed, the byte added will be { 0x20 }.</example>
		/// </summary>
		/// <typeparam name="T">The type of the number, used to know how many bytes will be pushed</typeparam>
		/// <param name="value">Value to be pushed</param>
		/// <returns>Reference to the buffer maker object (allows chaining methods)</returns>
		/// <exception cref="runtime_error">Occurs if the input types doesn't have 1 byte</exception>
		template <typename T>
		Serializer &push_byte(T value)
		{
			check_size(value, 1);
			return this->push_little_endian(value);
		}

		/// <summary>
		/// Get the buffer that is being constructed.
		/// <para>Before returning the buffer, the buffer's size will be placed in each deferred spot.</para>
		/// </summary>
		/// <returns>Vector with the inserted bytes</returns>
		const std::vector<unsigned char> &get_buffer()
		{
			this->insert_buffer_sizes();

			return this->buffer;
		}

		/// <summary>
		/// Get the current buffer size.
		/// </summary>
		/// <returns>Current buffer size</returns>
		size_t get_buffer_size()
		{
			return this->buffer.size();
		}

		/// <summary>
		/// Print the internal buffer specifying the size and separating each byte using <paramref name="sep"/> character.
		/// <para>Before printing the buffer, the buffer's size will be placed in each deferred spot.</para>
		/// </summary>
		/// <param name="sep">Character to use as separator</param>
		void print_buffer(char sep = ' ')
		{
			this->insert_buffer_sizes();

			printf("Buffer with %zd bytes long:\n", this->buffer.size());
			for (int i = 0; i < this->buffer.size(); i++)
			{
				if (i)
					printf("%c", sep);
				printf("%02x", this->buffer[i]);
			}
			printf("\n");
		}

		/// <summary>
		/// Get a string representation of the internal buffer.
		/// <para>Represent each byte as 2 characters hex value (0xff -> 'ff', 0x1 -> '01')</para>
		/// </summary>
		/// <returns>String representation of the internal buffer</returns>
		std::string get_buffer_string()
		{
			return make_buffer_string(this->get_buffer());
		}

		/// <summary>
		/// Marks the current buffer location as a place to receive the final buffer size as 4 bytes in Big-Endian.
		/// <para>
		/// For example, suppose the current buffer at the moment of the method call is: <para>{ 0x01, 0x02, 0x03 }</para>
		/// Suppose now that you inserted 3 more 0xff bytes after, the final buffer will be: <para>{ 0x01, 0x02, 0x03, 0x00, 0x00, 0x00, 0x0a 0xff, 0xff, 0xff }</para>
		/// </para>
		/// </summary>
		/// <returns>Reference to the buffer maker object (allows chaining methods)</returns>
		Serializer &defer_buffer_size_32_big_endian()
		{
			this->deferred_sizes.push_back({this->buffer.size(), 4, Endianness::BIG});

			return this->push_32_big_endian(0x00000000);
		}

	private:
		void insert_buffer_sizes()
		{
			size_t buffer_size = this->buffer.size();

			for (auto &deferred : this->deferred_sizes)
			{
				size_t index = deferred.index;

				for (int i = deferred.number_of_bytes - 1; i >= 0; i--)
					this->buffer[index++] = (buffer_size >> (i * 8)) & 0xff;
			}
		}

	private:
		enum class Endianness
		{
			BIG,
			LITTLE
		};

		struct Deferred_Buffer_Size
		{
			size_t index;
			unsigned char number_of_bytes;
			Endianness endianness;
		};

		std::vector<unsigned char> buffer;
		std::vector<Deferred_Buffer_Size> deferred_sizes;
	};

}