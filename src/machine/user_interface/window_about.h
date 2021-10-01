#pragma once

#include "window.h"
class WindowAbout
{
private:
	static const int win_width = 500;
	static const int win_height = 150;

public:
	Window window;

	void Init(SDL_Window* window_main);
};