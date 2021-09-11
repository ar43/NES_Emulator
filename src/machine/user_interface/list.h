#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <functional>
#include "element.h"

class Text;

class List : public Element
{
private:

	Text** elements;
public:
	List(SDL_Renderer* renderer, int x, int y, int w, int h)
	{
		this->renderer = renderer;
		SetRect(x, y, w, h * 14);
		elements = new Text * [h];
	}
	void HandleEvent(SDL_Event* e);
	void Render();
};