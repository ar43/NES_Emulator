#include "window_about.h"

void WindowAbout::Init(SDL_Window* window_main)
{
	window.Init("About", win_width, win_height, win_width, win_height, SDL_WINDOW_HIDDEN);

	window.AddText(10, 10, "NES Emulator made for FRI diploma thesis (preview version)", 14);
	window.AddText(10, 40, "F1 - toggle pause", 14);
	window.AddText(10, 60, "F2 - toggle speedup", 14);
	window.AddText(10, 80, "F3 - reset rom", 14);
	window.AddText(10, 100, "F4 - toggle force rendering (use only if nothing is rendered)", 14);
	window.AddText(10, 120, "F5 - toggle mute sound", 14);
}
