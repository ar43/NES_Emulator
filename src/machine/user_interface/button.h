#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <functional>
#include "element.h"

class Text;

class Button : public Element
{
private:
	Text *text_obj = nullptr;
	std::string text = "Empty";
	bool pressed = false;
public:
	Button(SDL_Renderer *renderer, int x, int y, int w, int h, std::string text, TTF_Font *font, std::function<void()> OnClick);
	void HandleEvent(SDL_Event* e);
	void Render(SDL_Renderer* renderer);
	//void (*OnClick)() = nullptr;
	std::function<void()> OnClick;
};