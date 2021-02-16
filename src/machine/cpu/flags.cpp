#include "flags.h"

namespace flags
{
	std::string to_string(Flags flag)
	{
		switch (flag)
		{
		case Flags::C:
			return "C";
		case Flags::Z:
			return "Z";
		case Flags::I:
			return "I";
		case Flags::D:
			return "D";
		case Flags::B:
			return "B";
		case Flags::O:
			return "O";
		case Flags::N:
			return "N";
		default:
			return "ERROR FLAG";
		}
	}
}