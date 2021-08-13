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

	int size = 16;
public:
	Text(SDL_Renderer *renderer, int x, int y, std::string text, TTF_Font *font, int size)
	{
		TTF_CloseFont(font);
		this->size = size;
		font = TTF_OpenFont( FONT_PATH, size );
		SetColor(0, 0, 0);
		SetPosition(x, y);
		SetText(renderer, text, font);
	}
	Text(SDL_Renderer *renderer, int x, int y, std::string text, TTF_Font *font, bool offset, int w, int h)
	{
		TTF_CloseFont(font);
		font = TTF_OpenFont( FONT_PATH, size );
		SetColor(0, 0, 0);
		SetPosition(x, y);
		SetText(renderer, text, font,offset,w,h);
	}
	void HandleEvent(SDL_Event* e) {};
	void Render(SDL_Renderer *renderer);
	void SetText(SDL_Renderer *renderer, std::string text, TTF_Font *font, bool offset = false, int w = 0, int h = 0);
	std::string GetText();
};