#pragma once
#include <SDL.h>
#include <array>
#include "palette.h"
#include "../../misc/constants.h"

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

	
	void RenderStart(Memory *mem);
	void RenderEnd();
	void Render(Memory *mem);
	void DrawChrRom(Memory *mem);

	void DrawBackgroundTile(Memory *mem, uint8_t bank, uint8_t index, SDL_Color *color_pointer, int x, int y);
	void DrawBackground(Memory* mem);
	void GetBackgroundMetaTileColor(Memory *mem, SDL_Color *color, int x, int y, int nametable);

	void DrawSprites(Memory* mem, bool behind);
	void DrawSprite(Memory* mem, uint8_t bank, uint8_t index, uint8_t palette_id, bool flip_h, bool flip_v, int x, int y);

	void BuildPixelValues(Memory *mem);
	void BuildPixelValue(Memory* mem, uint8_t bank, uint8_t index);

	void SetScale(uint8_t scale);
	uint8_t GetScale();

	SDL_Window* GetWindow();

	SDL_Event e;
private:
	uint8_t scale = 4;
	SDL_Window* window;
	SDL_Texture* texture;
	SDL_Surface* surface;
	SDL_Renderer* renderer;
	
	//uint32_t* pixels;
};