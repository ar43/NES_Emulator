#pragma once
#include <string>

namespace flags
{
	enum class Flags
	{
		C,
		Z,
		I,
		D,
		UNUSED,
		B,
		V,
		N,
		NUM_FLAGS
	};

	std::string to_string(Flags flag);
}