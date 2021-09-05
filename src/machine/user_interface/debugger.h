#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <array>
#include "window.h"

struct DebugData
{
	std::array<std::string, 0x10000> code;
	int known_bytes = 0;
	int all_bytes = -1;
};

class Debugger
{
private:
	
	static const int win_width = 300;
	static const int win_height = 300;
public:
	Window window;
	void Init(TTF_Font *font);
	void DrawBackground(SDL_Renderer* renderer);
	void Button1Click();
	void Checkbox1Click(bool *new_state);
	void Open();
	void Close();

	DebugData debug_data;
	bool* debug_mode = nullptr;
};

