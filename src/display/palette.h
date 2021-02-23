#pragma once
#include <SDL.h>
#include <array>
#include <string>

class Palette
{
public:
	void GetColor(SDL_Color *color, uint8_t index);
	bool Load(std::string name);
private:
	std::array<uint8_t, 192> palette;
};