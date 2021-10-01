#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "window.h"

class Button;
struct MachineStatus;

class Controls
{
private:
	static const int win_width = 240;
	static const int win_height = 650;

	Button* button_keymap[2][8];
	void OnButtonClick(int joynum, int butnum);
	void Refresh(bool cancel = false);
public:
	Window window;

	void Init(SDL_Window* window_main);

	void Open();
	void Close();
	void Update();

	SDL_Keycode (* keymaps)[8];

	MachineStatus *machine_status;
	
};