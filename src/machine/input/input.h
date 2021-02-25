#pragma once
#pragma once
#include <SDL.h>

struct MachineStatus;

class Input
{
public:
	void Poll(MachineStatus *machine_status, SDL_Window *window);
	void PollPause(MachineStatus *machine_status);
private:
	SDL_Event e;
};