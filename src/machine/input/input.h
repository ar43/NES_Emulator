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
	Input();
	void Poll(MachineStatus *machine_status, UserInterface* ui);
	void PollMain(MachineStatus *machine_status, UserInterface* ui);
	void PollDebugger(MachineStatus *machine_status, UserInterface* ui);
	void HandleMenuBar(MachineStatus* machine_status, UserInterface *ui, WORD param);
	Joypad joypad[2];

	SDL_Keycode keymaps[2][Joypad::num_keys];
private:
	SDL_Event e;
};