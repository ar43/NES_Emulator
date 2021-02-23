#pragma once
//#include <Windows.h>
#include <cstdint>
#include "timer.h"
#include <SDL.h>
#include <Windows.h>

class Frame
{
private:
	LARGE_INTEGER currentTime, previousTime, previousTime2, deltaTime, deltaTime2;
	LARGE_INTEGER frequency;
	uint32_t frames;
	double delta, delta2;
	void ran_frame();
public:
	void start();
	void end(SDL_Window* window);
	void end_f();
	void init();
	double getDelta();
	LARGE_INTEGER getTimeRaw();
	double getTime();
	float getTime_s();
	uint32_t getFrame();
	Timer capTimer;
};