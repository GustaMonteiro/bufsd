#pragma once

#include <vector>

#include "bufsd/deserializer.h"

namespace bufsd
{
	template <typename T>
	struct Deserializable
	{
		/// <summary>
		/// Fills the structure of an already created object with deserializer's bytes.
		/// <para>Since it uses the deserializer's buffer, it moves the cursor forward.
		/// </summary>
		/// <param name="deserializer">The deserializer contaning the bytes that will be used</param>
		virtual void fill_from_bytes(Deserializer &deserializer) = 0;

		/// <summary>
		/// Fills the structure of an already created object with a vector of bytes.
		/// </summary>
		/// <param name="buffer">The bytes to deserialize</param>
		// virtual void fill_from_bytes(const std::vector<unsigned char> &buffer) = 0;
		void fill_from_bytes(const std::vector<unsigned char> &buffer)
		{
			Deserializer deserializer{buffer};

			this->fill_from_bytes(deserializer);
		}

		/// <summary>
		/// Creates a new object with a vector of bytes.
		/// </summary>
		/// <param name="buffer">The bytes to deserialize</param>
		/// <returns>The newly created object</returns>
		static T from_bytes(const std::vector<unsigned char> &buffer)
		{
			T object{};
			Deserializer deserializer{buffer};
			object.fill_from_bytes(deserializer);

			return object;
		}

		/// <summary>
		/// Creates a new object with deserializer's bytes.
		/// </summary>
		/// <param name="deserializer">The deserializer contaning the bytes that will be used</param>
		/// <returns>The newly created object</returns>
		static T from_bytes(Deserializer &deserializer)
		{
			T object{};

			object.fill_from_bytes(deserializer);

			return object;
		}
	};
}