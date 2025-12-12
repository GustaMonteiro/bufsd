#include "bufsd/deserializer.h"
#include "bufsd/utils.h"

namespace bufsd
{
	static std::string make_error_message(size_t requested, size_t remaining)
	{
		std::stringstream ss;
		ss << "Tried to get/skip " << requested << " byte(s), but there's only " << remaining << " byte(s) remaining";
		return ss.str();
	}

	unsigned char Deserializer::get_byte()
	{
		return (unsigned char)this->get_big_endian(1);
	}

	std::vector<unsigned char> Deserializer::get_buffer(size_t size)
	{
		this->assert_is_available(size);

		std::vector<unsigned char>::iterator begin = this->buffer.begin() + this->cursor;
		std::vector<unsigned char>::iterator end = begin + size;

		this->cursor += size;
		this->update_remaining();

		return std::vector<unsigned char>(begin, end);
	}

	unsigned short Deserializer::get_16_big_endian()
	{
		return (unsigned short)this->get_big_endian(2);
	}

	unsigned int Deserializer::get_32_big_endian()
	{
		return (unsigned int)this->get_big_endian(4);
	}

	unsigned long long Deserializer::get_64_big_endian()
	{
		return this->get_big_endian(8);
	}

	unsigned long long Deserializer::get_big_endian(size_t amount_of_bytes)
	{
		assert_is_available(amount_of_bytes);

		unsigned long long value = 0;

		for (int i = (int)amount_of_bytes - 1; i >= 0; i--)
			value |= (unsigned long long)this->buffer[this->cursor++] << 8 * i;

		this->update_remaining();

		return value;
	}

	unsigned short Deserializer::get_16_little_endian()
	{
		return (unsigned short)this->get_little_endian(2);
	}

	unsigned int Deserializer::get_32_little_endian()
	{
		return (unsigned int)this->get_little_endian(4);
	}

	unsigned long long Deserializer::get_64_little_endian()
	{
		return this->get_little_endian(8);
	}

	unsigned long long Deserializer::get_little_endian(size_t amount_of_bytes)
	{
		assert_is_available(amount_of_bytes);

		unsigned long long value = 0;

		for (int i = 0; i < amount_of_bytes; i++)
			value |= (unsigned long long)this->buffer[this->cursor++] << 8 * i;

		this->update_remaining();

		return value;
	}

	Deserializer::Deserializer(const std::vector<unsigned char> &buffer)
		: buffer(buffer), buffer_size(buffer.size()), remaining(buffer.size())
	{
	}

	void Deserializer::assert_is_available(size_t amount_of_bytes) const
	{
		if (this->remaining < amount_of_bytes)
			throw std::runtime_error(make_error_message(amount_of_bytes, this->remaining));
	}

	void Deserializer::update_remaining()
	{
		if (this->cursor >= this->buffer_size || this->cursor < 0)
			this->remaining = 0;
		else
			this->remaining = this->buffer_size - this->cursor;
	}

	size_t Deserializer::get_cursor() const
	{
		return this->cursor;
	}

	size_t Deserializer::get_remaining() const
	{
		return this->remaining;
	}

	size_t Deserializer::get_buffer_size() const
	{
		return this->buffer_size;
	}

	void Deserializer::skip(size_t amount_of_bytes)
	{
		this->assert_is_available(amount_of_bytes);

		this->cursor += amount_of_bytes;
		this->update_remaining();
	}

	void Deserializer::reset_cursor()
	{
		this->cursor = 0;
		this->update_remaining();
	}

	void Deserializer::set_cursor(size_t position)
	{
		this->reset_cursor();
		this->skip(position);
	}

	void Deserializer::print_buffer(char sep)
	{
		printf("Buffer with %zd bytes long:\n", this->buffer.size());
		for (int i = 0; i < this->buffer.size(); i++)
		{
			if (i)
				printf("%c", sep);
			printf("%02x", this->buffer[i]);
		}
		printf("\n");
	}

	std::string Deserializer::get_buffer_string()
	{
		return bufsd::make_buffer_string(this->buffer);
	}
}