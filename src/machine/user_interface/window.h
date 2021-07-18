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
	SDL_Color color = { 0xff,0xff,0xff };
public:

	bool no_update = false;

	bool IsFocused() { return focus; }
	bool IsShown() { return shown; }
	bool IsMinimized() { return minimized; }

	void Show() { SDL_ShowWindow(window); }
	void Hide() { SDL_HideWindow(window); }
	void Toggle();

	void Open();
	void Init(std::string window_name, int width, int height, int rend_width, int rend_height, Uint32 flags = 0);
	void Update();
	SDL_Window* GetWindow();
	SDL_Renderer* GetRenderer();
	void HandleEvent(SDL_Event* e, bool *request_exit);
};