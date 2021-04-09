#pragma once
#include <SDL.h>
#include "menu_bar.h"

class UserInterface
{
private:
	SDL_Window* main_window;
	
public:

	SDL_Window* GetWindow();
	void SetScale(uint8_t scale);
	uint8_t GetScale();
	void Init();

	uint8_t scale = 4;

	MenuBar menu_bar;
};