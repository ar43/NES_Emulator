#include "palette.h"
#include "../logger/logger.h"
#include <fstream>
#include <cassert>

bool Palette::Load(std::string name)
{
	std::string path = "data/" + name + ".pal";
	std::ifstream ifs(path, std::ifstream::in | std::ifstream::binary);
	if (!ifs.is_open())
	{
		logger::PrintLine(logger::LogType::ERROR, "Palette file does not exist! " + path);
		return false;
	}

	ifs.read((char*)palette.data(), palette.size());
	if (!ifs)
	{
		logger::PrintLine(logger::LogType::ERROR, "Bad .pal file format! File too short.");
		ifs.close();
		return false;
	}

	ifs.close();
	return true;
}

void Palette::GetColor(SDL_Color *color, uint8_t index)
{
	assert(color != nullptr && index >= 0 && index <= 63);

	index *= 3;

	color->r = palette[index];
	color->g = palette[index+1];
	color->b = palette[index+2];
	color->a = 0xFF;
}