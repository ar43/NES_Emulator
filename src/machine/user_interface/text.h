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
public:
	Text(SDL_Renderer *renderer, int x, int y, std::string text, int size)
	{
		this->size = size;
		TTF_Font *font = TTF_OpenFont( FONT_PATH, size );
		this->font = font;
		SetColor(0, 0, 0);
		SetPosition(x, y);
		SetText(renderer, text);
	}
	Text(SDL_Renderer *renderer, int x, int y, std::string text, bool offset, int w, int h)
	{
		TTF_Font *font = TTF_OpenFont( FONT_PATH, size );
		this->font = font;
		SetColor(0, 0, 0);
		SetPosition(x, y);
		SetText(renderer, text, offset,w,h);
	}
	void HandleEvent(SDL_Event* e) {};
	void Render(SDL_Renderer *renderer);
	void SetText(SDL_Renderer *renderer, std::string text, bool offset = false, int w = 0, int h = 0);
	std::string GetText();
};