#pragma once

#include <vector>
#include <string>

namespace bufsd
{
	class Deserializer
	{
	public:
		/// <summary>
		/// Get the next 1 byte of the buffer.
		/// <para>Moves the cursor 1 byte forward.</para>
		/// </summary>
		/// <exception cref="runtime_error">Occurs when there is no byte left in the buffer</exception>
		/// <returns>unsigned char of the next byte of buffer</returns>
		unsigned char get_byte();

		/// <summary>
		/// Get the <paramref name="size"/> next bytes of the buffer.
		/// <para>Moves the cursor the same amount of bytes.</para>
		/// </summary>
		/// <param name="size">Amount of bytes to get from buffer</param>
		/// <exception cref="runtime_error">Occurs when there is not enough bytes left in the buffer</exception>
		/// <returns>Vector with the requested amount of bytes</returns>
		std::vector<unsigned char> get_buffer(size_t size);

		/// <summary>
		/// Get the next 2 bytes of the buffer in Big-Endian (not inverting the order).
		/// <para>Moves the cursor 2 bytes forward.</para>
		/// </summary>
		/// <exception cref="runtime_error">Occurs when there is not enough bytes left in the buffer</exception>
		/// <returns>unsigned short of the next 2 bytes of the buffer</returns>
		unsigned short get_16_big_endian();

		/// <summary>
		/// Get the next 4 bytes of the buffer in Big-Endian (not inverting the order).
		/// <para>Moves the cursor 4 bytes forward.</para>
		/// </summary>
		/// <exception cref="runtime_error">Occurs when there is not enough bytes left in the buffer</exception>
		/// <returns>unsigned int of the next 4 bytes of the buffer</returns>
		unsigned int get_32_big_endian();

		/// <summary>
		/// Get the next 8 bytes of the buffer in Big-Endian (not inverting the order).
		/// <para>Moves the cursor 8 bytes forward.</para>
		/// </summary>
		/// <exception cref="runtime_error">Occurs when there is not enough bytes left in the buffer</exception>
		/// <returns>unsigned long long of the next 8 bytes of the buffer</returns>
		unsigned long long get_64_big_endian();

		/// <summary>
		/// Get the next 2 bytes of the buffer in Little-Endian (inverting the order).
		/// <para>Moves the cursor 2 bytes forward.</para>
		/// </summary>
		/// <exception cref="runtime_error">Occurs when there is not enough bytes left in the buffer</exception>
		/// <returns>unsigned short of the next 2 bytes of the buffer</returns>
		unsigned short get_16_little_endian();

		/// <summary>
		/// Get the next 4 bytes of the buffer in Little-Endian (inverting the order).
		/// <para>Moves the cursor 4 bytes forward.</para>
		/// </summary>
		/// <exception cref="runtime_error">Occurs when there is not enough bytes left in the buffer</exception>
		/// <returns>unsigned int of the next 4 bytes of the buffer</returns>
		unsigned int get_32_little_endian();

		/// <summary>
		/// Get the next 8 bytes of the buffer in Little-Endian (inverting the order).
		/// <para>Moves the cursor 8 bytes forward.</para>
		/// </summary>
		/// <exception cref="runtime_error">Occurs when there is not enough bytes left in the buffer</exception>
		/// <returns>unsigned long long of the next 8 bytes of the buffer</returns>
		unsigned long long get_64_little_endian();

		/// <summary>
		/// Get the current cursor position, starting from 0.
		/// </summary>
		/// <returns>Current cursor position</returns>
		size_t get_cursor() const;

		/// <summary>
		/// Get the remaining amount of bytes in the buffer.
		/// </summary>
		/// <returns>Remaining amount of bytes</returns>
		size_t get_remaining() const;

		/// <summary>
		/// Get the full buffer size.
		/// </summary>
		/// <returns>Full buffer size</returns>
		size_t get_buffer_size() const;

		/// <summary>
		/// Move the cursor <paramref name="amount_of_bytes"/> bytes forward.
		/// </summary>
		/// <exception cref="runtime_error">Occurs when there is not enough bytes to skip in the buffer</exception>
		/// <param name="amount_of_bytes">Number of bytes to move the cursor</param>
		void skip(size_t amount_of_bytes);

		/// <summary>
		/// Move the cursor to the initial position (0).
		/// </summary>
		void reset_cursor();

		/// <summary>
		/// Move the cursor to a given position
		/// </summary>
		/// <param name="position"></param>
		void set_cursor(size_t position);

		/// <summary>
		/// Print the internal buffer specifying the size and separating each byte using <paramref name="sep"/> character.
		/// </summary>
		/// <param name="sep">Character to use as separator</param>
		void print_buffer(char sep = ' ');

		/// <summary>
		/// Get a string representation of the internal buffer.
		/// <para>Represent each byte as 2 characters hex value (0xff -> 'ff', 0x1 -> '01')</para>
		/// </summary>
		/// <returns>String representation of the internal buffer</returns>
		std::string get_buffer_string();

		/// <summary>
		/// Constructs a new buffer deserializer with <paramref name="buffer"/> as internal buffer.
		/// </summary>
		/// <param name="buffer"></param>
		Deserializer(const std::vector<unsigned char> &buffer);

	private:
		unsigned long long get_big_endian(size_t amount_of_bytes);
		unsigned long long get_little_endian(size_t amount_of_bytes);
		void assert_is_available(size_t amount_of_bytes) const;
		void update_remaining();

	private:
		std::vector<unsigned char> buffer;

		size_t cursor = 0;
		size_t buffer_size = 0;
		size_t remaining = 0;
	};
}