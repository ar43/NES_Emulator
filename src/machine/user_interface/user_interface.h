#pragma once
#include <SDL.h>
#include <Windows.h>
#include <string>
#include "menu_bar.h"
#include "window_debugger.h"
#include "window_controls.h"

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
	void UpdateAll();
	Window window;

	uint8_t scale = 4;

	MenuBar menu_bar;
	WindowDebugger debugger;
	WindowControls controls;
};