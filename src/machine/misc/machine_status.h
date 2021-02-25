#pragma once

struct MachineStatus
{
	bool running = true;
	bool paused = false;
	bool* speedup;
	int reset = 2;
};