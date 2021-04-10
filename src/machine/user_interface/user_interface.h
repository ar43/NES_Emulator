#pragma once
#include <SDL.h>
#include <Windows.h>
#include <string>
#include "menu_bar.h"


class UserInterface
{
private:
	SDL_Window* main_window;
	
public:

	static HWND GetSDLWinHandle(SDL_Window* win);
	static std::string GetROMPath(SDL_Window* window);

	SDL_Window* GetWindow();
	void SetScale(uint8_t scale);
	uint8_t GetScale();
	void Init();

	uint8_t scale = 4;

	MenuBar menu_bar;
};