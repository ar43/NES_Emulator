#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "window.h"

class Debugger
{
private:
	
	static const int win_width = 300;
	static const int win_height = 300;
public:
	Window window;
	void Open();
	void Init(TTF_Font *font);
	static void DrawBackground(SDL_Renderer* renderer);
	static void Button1Click();
};