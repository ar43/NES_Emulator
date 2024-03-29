#pragma once
#include "element.h"
#include "../misc/constants.h"
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>


class Text : public Element
{
private:
	std::string text;
	SDL_Texture *texture = NULL;
	TTF_Font* font;

	int size = 16;
	
	int w = 0;
	int h = 0;
public:
	Text(SDL_Renderer *renderer, int x, int y, std::string text, int size)
	{
		this->renderer = renderer;
		this->size = size;
		TTF_Font *font = TTF_OpenFont( FONT_PATH, size );
		this->font = font;
		SetColor(0, 0, 0);
		SetPosition(x, y);
		SetText(text);
	}
	Text(SDL_Renderer *renderer, int x, int y, std::string text, bool offset, int w, int h)
	{
		this->renderer = renderer;
		TTF_Font *font = TTF_OpenFont( FONT_PATH, size );
		this->font = font;
		SetColor(0, 0, 0);
		SetPosition(x, y);
		SetText(text, offset,w,h);
	}
	bool HandleEvent(SDL_Event* e, Uint32* current_active_element) { return false; };
	void Render(Uint32* current_active_element);
	void SetText(std::string text, bool offset = false, int w = 0, int h = 0);
	std::string GetText();
	int GetHeight();
	int GetWidth();
	int SimulateWidth(std::string in_text);
	int render_limit = 0;
};