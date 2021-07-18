#pragma once
#include <SDL.h>
#include <string>

class Window
{
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	Uint32 window_id;
	
	bool shown = false;
	bool focus = false;
	bool minimized = false;
public:

	bool IsFocused() { return focus; }
	bool IsShown() { return shown; }
	bool IsMinimized() { return minimized; }


	void Open();
	void Init(std::string window_name, int width, int height, int width2, int height2);
	SDL_Window* GetWindow();
	SDL_Renderer* GetRenderer();
	void HandleEvent(SDL_Event* e, bool *request_exit);
};