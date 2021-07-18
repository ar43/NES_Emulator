#pragma once
#include <SDL.h>
#include <Windows.h>
#include <string>
#include "menu_bar.h"
#include "debugger.h"

class Window;

class UserInterface
{
private:
	
public:

	static HWND GetSDLWinHandle(SDL_Window* win);
	static std::string GetROMPath(SDL_Window* window);

	void SetScale(uint8_t scale);
	uint8_t GetScale();
	void InitSDL();
	void Init();
	Window window;

	uint8_t scale = 4;

	MenuBar menu_bar;
	Debugger debugger;
};