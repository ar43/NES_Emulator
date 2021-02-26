#pragma once
#include <SDL.h>
#include "joypad.h"

struct MachineStatus;
class Display;
class Memory;

class Input
{
public:
	void Poll(MachineStatus *machine_status, SDL_Window *window, Display* display, Memory *memory);
	void PollPause(MachineStatus *machine_status);
	Joypad joypad[2];
private:
	SDL_Event e;
};