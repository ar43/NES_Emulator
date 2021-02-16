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
		B,
		O,
		N,
		NUM_FLAGS
	};

	std::string to_string(Flags flag);
}