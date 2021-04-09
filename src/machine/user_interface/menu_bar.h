#pragma once
#include <SDL.h>
#include <Windows.h>

enum class MenuBarID
{
	LOADROM,
	ABOUT,
	EXIT,
	CONTROLS,
	DEBUG,
	RESET,
	PAUSE,
};

class MenuBar
{
public:
	HMENU hHelp;
	HMENU hGame;
	HMENU hTools;
	HMENU hOptions;
	HMENU hFile;
	HMENU hMenuBar;

	HWND GetSDLWinHandle(SDL_Window* win);
	void Activate(HWND windowRef);
	void Init(SDL_Window* win);
	void AdjustWindowSize(SDL_Window* win);
};