#pragma once
#include <SDL.h>
#include <Windows.h>

enum class MenuBarID
{
	LOAD_ROM,
	ABOUT,
	EXIT,
	CONTROLS,
	DEBUG,
	RESET,
	PAUSE,
	POWER_OFF,
};

class MenuBar
{
private:
	HMENU hHelp;
	HMENU hGame;
	HMENU hTools;
	HMENU hOptions;
	HMENU hFile;
	HMENU hMenuBar;
public:
	void Activate(HWND windowRef);
	void Init(SDL_Window* win);
	void AdjustWindowSize(SDL_Window* win);
};