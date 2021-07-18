#pragma once
#include <SDL.h>
#include <Windows.h>
#include "joypad.h"

struct MachineStatus;
class Display;
class Bus;
class UserInterface;

class Input
{
public:
	void Poll(MachineStatus *machine_status, UserInterface* ui);
	void PollMain(MachineStatus *machine_status, UserInterface* ui);
	void PollDebugger(MachineStatus *machine_status, UserInterface* ui);
	void HandleMenuBar(MachineStatus* machine_status, SDL_Window * main_window, WORD param);
	Joypad joypad[2];
private:
	SDL_Event e;
};