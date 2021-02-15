#include "memory.h"
#include "../../logger/logger.h"

void Memory::Write(size_t loc, uint8_t byte)
{
	if (loc < 0 || loc > data.size())
	{ 
		logger::PrintLine(logger::LogType::ERROR, "Memory::Write - Tried to write out of bounds: " + std::to_string(loc));
		return;
	}

	data[loc] = byte;
}

uint8_t Memory::Read(size_t loc)
{
	if (loc < 0 || loc > data.size())
	{ 
		logger::PrintLine(logger::LogType::ERROR, "Memory::Read - Tried to read out of bounds: " + std::to_string(loc));
		return 0;
	}

	return data[loc];
}