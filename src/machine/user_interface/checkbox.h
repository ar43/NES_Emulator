#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "element.h"

class Text;

class Checkbox : public Element
{
private:
	inline static SDL_Texture* texture = nullptr;
	Text *text_obj = nullptr;
	std::string text = "Empty";
	bool pressed = false;
	bool state = false;
	int w = 14;
	int h = 14;
public:
	Checkbox(SDL_Renderer *renderer, int x, int y, std::string text, TTF_Font *font, void (*OnClick)(bool *new_state));
	void HandleEvent(SDL_Event* e);
	void Render(SDL_Renderer* renderer);
	void (*OnClick)(bool *new_state) = nullptr;

	static void InitTexture(SDL_Renderer *renderer);
};