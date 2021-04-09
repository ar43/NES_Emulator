#pragma once
#include <SDL.h>
#include <array>
#include "palette.h"
#include "../../misc/constants.h"

class Bus;
struct PpuRegisters;

class Display
{
public:
	Display() 
	{
		window = nullptr;
		renderer = nullptr;
		surface = nullptr;
		texture = nullptr;
	}
	~Display();
	bool Init(SDL_Window* window, uint8_t* scale);

	Palette palette;
	
	void Render(PpuRegisters *ppu_registers, uint8_t * oam_data);

	void SetScale(uint8_t scale);
	uint8_t GetScale();

	void CheckRebuild(Bus *bus, PpuRegisters *ppu_registers);

	void DrawBackgroundLineHSA(Bus *bus, uint8_t x_shift, uint8_t y_shift, int nametable, uint8_t bank, int line, bool toggle, bool show_left);
	void DrawBackgroundLineVSB(Bus *bus, uint8_t x_shift, uint8_t y_shift, int nametable, uint8_t bank, int line, bool toggle, bool show_left, int nametable_mirroring);
	void DrawBackgroundLineVSA(Bus* bus, uint8_t x_shift, uint8_t y_shift, int nametable, uint8_t bank, int line, bool toggle, bool show_left, int nametable_mirroring);
	void DrawBackgroundLineHSB(Bus *bus, uint8_t x_shift, uint8_t y_shift, int nametable, uint8_t bank, int line, bool toggle, bool show_left, int nametable_mirroring);

	void BuildPixelValues(Bus *bus);
	void BuildPixelValue(Bus *bus, uint8_t bank, uint8_t index);

	//void DrawSpritesLine(Memory* mem, bool behind, int line);
	
	void DrawSprites(PpuRegisters *ppu_registers, uint8_t *oam_data, int scanline);

	SDL_Window* GetWindow();
	void RenderStart(Bus *bus);
	SDL_Surface* surface;
	SDL_Event e;
	uint8_t pixel_values[2][256 * 64];
private:
	uint8_t* scale = nullptr;
	SDL_Window* window;
	SDL_Texture* texture;
	
	SDL_Renderer* renderer;

	//void DrawSpriteLine(Memory* mem, uint8_t bank, uint8_t index, uint8_t palette_id, bool flip_h, bool flip_v, int x, int line);

	void RenderEnd();
	void DrawChrRom(Bus *bus);

	void DrawBackgroundTileLine(uint8_t bank, uint8_t index, SDL_Color *color_pointer, uint8_t read_line, int x, int line, bool show_background_left);
	void GetBackgroundMetaTileColor(Bus *bus, SDL_Color *color, int x, int y, int nametable);

	
	bool DrawSprite(uint8_t bank, uint8_t index, uint8_t palette_id, bool flip_h, bool flip_v, int x, int y, bool draw_left, bool behind, int sprite_num, int offset, bool x16);
	
	//uint32_t* pixels;
};