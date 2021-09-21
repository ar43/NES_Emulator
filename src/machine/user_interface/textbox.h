#pragma once
#include "element.h"
#include <string>

class Text;

class Textbox : public Element
{
private:
	std::string text = "Empty";
	Text* text_obj = nullptr;
	bool pressed = false;

	const static int text_size = 14;
public:
	Textbox(SDL_Renderer *renderer, int x, int y, int w, std::string text);
	bool HandleEvent(SDL_Event* e, Uint32 *current_active_element);
	void Render(Uint32* current_active_element);
};