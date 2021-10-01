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
	bool hover = false;

	const static Uint8 base_color = 0xd1;
public:
	Button(SDL_Renderer *renderer, int x, int y, int w, int h, std::string text, std::function<void()> OnClick);
	void SetText(std::string text);
	bool HandleEvent(SDL_Event* e, Uint32 *current_active_element);
	void Render(Uint32* current_active_element);
	int GetWidth();
	//void (*OnClick)() = nullptr;
	std::function<void()> OnClick;

	void SetActive(bool active);
	bool listen = false;
	bool read = false;
	SDL_Keycode key;
};