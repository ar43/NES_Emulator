#pragma once
#include <SDL.h>
#include "joypad.h"

struct MachineStatus;
class Display;
class Bus;

class Input
{
public:
	void Poll(MachineStatus *machine_status, SDL_Window *window, Display* display);
	void PollPause(MachineStatus *machine_status);
	Joypad joypad[2];
private:
	SDL_Event e;
};