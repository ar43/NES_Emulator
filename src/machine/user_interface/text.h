#pragma once
#include "element.h"
class Text : public Element
{
private:
public:
	void HandleEvent(SDL_Event* e) {};
	void Render(SDL_Renderer *renderer) {};
};