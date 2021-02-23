#pragma once
#include <SDL.h>
#include <array>
#include "palette.h"

constexpr uint32_t SCREEN_WIDTH = 256;
constexpr uint32_t SCREEN_HEIGHT = 240;
constexpr int PIXEL_PER_TILE = 64;
constexpr int TILE_WIDTH = 8;
constexpr int TILE_HEIGHT = 8;
constexpr int TILE_PER_BANK = 256;
constexpr int BYTES_PER_TILE = 16;

class Memory;

class Display
{
public:
	Display() 
	{
		window = nullptr;
		renderer = nullptr;
	}
	~Display();
	bool Init();

	Palette palette;

	
	void RenderStart();
	void RenderEnd();
	void Render(Memory *mem);
	void DrawChrRom(Memory *mem);

	void DrawBackgroundTile(Memory *mem, uint8_t bank, uint8_t index, SDL_Color *color_pointer, int x, int y);
	void DrawBackground(Memory* mem);
	void GetBackgroundMetaTileColor(Memory *mem, SDL_Color *color, int x, int y, int nametable);

	void BuildPixelValues(Memory *mem);

	void ProcessInput();

	void SetScale(uint8_t scale);
	uint8_t GetScale();

	SDL_Window* GetWindow();

	SDL_Event e;
	uint8_t pixel_values[2][TILE_PER_BANK * PIXEL_PER_TILE];
private:
	uint8_t scale = 4;
	SDL_Window* window;
	SDL_Texture* texture;
	SDL_Renderer* renderer;
	
	uint32_t* pixels;
};