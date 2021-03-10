#pragma once
#include <SDL.h>
#include <array>
#include <string>
#include "../../misc/constants.h"

class Memory;

class Palette
{
public:
	void GetColor(SDL_Color *color, uint8_t index);
	bool Load(std::string name);
	void LoadBackground(Memory* mem);
	void LoadSprite(Memory* mem);
	uint8_t background[4][3];
	uint8_t sprite[4][3];
	uint8_t universal_background;
private:
	uint8_t palette[PALETTE_SIZE];
};