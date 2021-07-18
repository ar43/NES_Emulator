#pragma once
#include <SDL.h>
#include "window.h"

class Debugger
{
private:
	
	static const int win_width = 300;
	static const int win_height = 300;
public:
	Window window;
	void Open();
	void Init();
};