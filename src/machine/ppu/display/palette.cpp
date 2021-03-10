#include "palette.h"
#include "../../../logger/logger.h"
#include "../../memory/memory.h"
#include <fstream>
#include <cassert>

bool Palette::Load(std::string name)
{
	std::string path = "data/" + name + ".pal";
	std::ifstream ifs(path, std::ifstream::in | std::ifstream::binary);
	if (!ifs.is_open())
	{
		logger::PrintLine(logger::LogType::INTERNAL_ERROR, "Palette file does not exist! " + path);
		return false;
	}

	ifs.read((char*)palette, PALETTE_SIZE);
	if (!ifs)
	{
		logger::PrintLine(logger::LogType::INTERNAL_ERROR, "Bad .pal file format! File too short.");
		ifs.close();
		return false;
	}

	ifs.close();
	return true;
}

void Palette::LoadBackground(Memory* mem)
{
	int counter = 0;
	universal_background = mem->ReadPPU(0x3F00);
	for (int i = 0x3F01; i <= 0x3F0F; i++)
	{
		if (i == 0x3F04 || i == 0x3F08 || i == 0x3F0C)
			continue;
		background[counter / 3][counter % 3] = mem->ReadPPU(i);
		counter++;
	}
}

void Palette::LoadSprite(Memory* mem)
{
	int counter = 0;
	for (int i = 0x3F11; i <= 0x3F1F; i++)
	{
		if (i == 0x3F14 || i == 0x3F18 || i == 0x3F1C)
			continue;
		sprite[counter / 3][counter % 3] = mem->ReadPPU(i);
		counter++;
	}
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