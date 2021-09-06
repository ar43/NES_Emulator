#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <functional>
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
	Checkbox(SDL_Renderer *renderer, int x, int y, std::string text, std::function<void(bool*)> OnClick);
	void HandleEvent(SDL_Event* e);
	void Render(SDL_Renderer* renderer);
	//void (*OnClick)(bool *new_state) = nullptr;
	std::function<void(bool*)> OnClick;

	static void InitTexture(SDL_Renderer *renderer);
};