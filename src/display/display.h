#pragma once
#include <SDL.h>
#include <array>
#include "palette.h"

constexpr uint32_t SCREEN_WIDTH = 256;
constexpr uint32_t SCREEN_HEIGHT = 240;

class Memory;

class Display
{
public:
	Display() 
	{
		window = nullptr;
		renderer = nullptr;
		std::fill(std::begin(pixels), std::end(pixels), 0);
	}
	~Display();
	bool Init();

	Palette palette;

	void DrawTile(Memory *mem, uint8_t bank, uint8_t index, int x, int y);
	void RenderStart();
	void RenderEnd();
	void Render(Memory *mem);
	void DrawChrRom(Memory *mem);
	void DrawBackground(Memory* mem);

	void SetScale(uint8_t scale);
	uint8_t GetScale();

	SDL_Event e;
private:
	uint8_t scale = 4;
	SDL_Window* window;
	SDL_Texture* texture;
	SDL_Renderer* renderer;
	
	std::array<uint32_t, SCREEN_HEIGHT* SCREEN_WIDTH> pixels;
};