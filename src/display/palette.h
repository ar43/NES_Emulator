#pragma once
#include <SDL.h>
#include <array>
#include <string>

class Memory;

class Palette
{
public:
	void GetColor(SDL_Color *color, uint8_t index);
	bool Load(std::string name);
	void LoadBackground(Memory* mem);
	uint8_t background[4][3];
	uint8_t universal_background;
private:
	std::array<uint8_t, 192> palette;
};