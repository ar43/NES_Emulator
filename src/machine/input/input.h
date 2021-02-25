#pragma once
#include <SDL.h>
#include "joypad.h"

struct MachineStatus;

class Input
{
public:
	void Poll(MachineStatus *machine_status, SDL_Window *window);
	void PollPause(MachineStatus *machine_status);
	Joypad joypad[2];
private:
	SDL_Event e;
};