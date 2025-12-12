#pragma once

#include <vector>
#include <string>
#include "bufsd/utils.h"

namespace bufsd
{
	struct Serializable
	{
		/// <summary>
		/// Serialize the object to a vector of bytes.
		/// <para>Every serializable class can be used with the Serializer</para>
		/// </summary>
		/// <returns>Vector with the serialized representation of the object</returns>
		virtual std::vector<unsigned char> serialize() const = 0;

		/// <summary>
		/// Get a string representation of the serialized object.
		/// </summary>
		/// <returns>String representation of the serialized object</returns>
		std::string to_string() const
		{
			return make_buffer_string(this->serialize());
		}
	};
}