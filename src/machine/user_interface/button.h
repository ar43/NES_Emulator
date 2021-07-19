#pragma once
#include <SDL.h>
#include "element.h"
class Button : public Element
{
private:
	

	bool pressed = false;
public:
	Button(int x, int y, int w, int h)
	{
		SetRect(x, y, w, h);
		SetColor(0, 0, 0);
	}
	void HandleEvent(SDL_Event* e);
	void Render(SDL_Renderer* renderer);
	void (*OnClick)() = 0;
};