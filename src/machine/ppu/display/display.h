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
	
	void Render(Memory *mem);

	void SetScale(uint8_t scale);
	uint8_t GetScale();

	void DrawBackgroundLineHSA(Memory* mem, uint8_t x_shift, int nametable, uint8_t bank, int line);
	void DrawBackgroundLineHSB(Memory* mem, uint8_t x_shift, int nametable, uint8_t bank, int line);

	void DrawSpritesLine(Memory* mem, bool behind, int line);
	

	SDL_Window* GetWindow();
	void RenderStart(Memory *mem);
	SDL_Surface* surface;
	SDL_Event e;
private:
	uint8_t scale = 4;
	SDL_Window* window;
	SDL_Texture* texture;
	
	SDL_Renderer* renderer;

	void DrawSpriteLine(Memory* mem, uint8_t bank, uint8_t index, uint8_t palette_id, bool flip_h, bool flip_v, int x, int line);

	void RenderEnd();
	void DrawChrRom(Memory *mem);

	void DrawBackgroundTile(Memory *mem, uint8_t bank, uint8_t index, SDL_Color *color_pointer, int x, int y);
	void DrawBackgroundTileLine(Memory *mem, uint8_t bank, uint8_t index, SDL_Color *color_pointer, int x, int line);
	//void DrawBackgroundTileOverride(Memory *mem, uint8_t bank, uint8_t index, SDL_Color *color_pointer, int x, int y);
	void DrawBackgroundHS(Memory* mem);
	void GetBackgroundMetaTileColor(Memory *mem, SDL_Color *color, int x, int y, int nametable);
	void DrawBackgroundHSA(Memory* mem, uint8_t x_shift, int nametable, uint8_t bank);
	void DrawBackgroundHSB(Memory* mem, uint8_t x_shift, int nametable, uint8_t bank);
	

	void DrawSprites(Memory* mem, bool behind);
	void DrawSprite(Memory* mem, uint8_t bank, uint8_t index, uint8_t palette_id, bool flip_h, bool flip_v, int x, int y);
	
	//uint32_t* pixels;
};