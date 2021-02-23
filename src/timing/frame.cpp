#include "frame.h"
#include <string>
#include <iomanip>
#include <sstream>

void Frame::start()
{
	/*
	currentTime = SDL_GetTicks();
	deltaTime = currentTime - previousTime;
	previousTime = currentTime;
	*/
	QueryPerformanceCounter(&currentTime);
	deltaTime.QuadPart = currentTime.QuadPart - previousTime.QuadPart;
	previousTime = currentTime;
	deltaTime.QuadPart *= 1000000;
	delta = (double)(deltaTime.QuadPart / frequency.QuadPart);


}

void Frame::ran_frame()
{
	/*
	currentTime = SDL_GetTicks();
	deltaTime = currentTime - previousTime;
	previousTime = currentTime;
	*/
	QueryPerformanceCounter(&currentTime);
	deltaTime2.QuadPart = currentTime.QuadPart - previousTime2.QuadPart;
	previousTime2 = currentTime;
	deltaTime2.QuadPart *= 1000000;
	delta2 = (double)(deltaTime2.QuadPart / frequency.QuadPart);


}

void Frame::init()
{
	frames = 0;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&previousTime);
	//previousTime = SDL_GetTicks();
	QueryPerformanceCounter(&currentTime);
	deltaTime.QuadPart = currentTime.QuadPart - previousTime.QuadPart;
	previousTime = currentTime;
	previousTime2 = currentTime;
	deltaTime.QuadPart *= 1000000;
	delta = (double)(deltaTime.QuadPart / frequency.QuadPart);
	deltaTime2.QuadPart *= 1000000;
	delta2 = (double)(deltaTime2.QuadPart / frequency.QuadPart);
	//SDL_Delay(10);
	capTimer.start(true,16.6666, this);
}

uint32_t Frame::getFrame()
{
	return frames;
}

double Frame::getDelta()
{
	return delta / 1000.0;
}

LARGE_INTEGER Frame::getTimeRaw()
{
	return currentTime;
}

double Frame::getTime()
{
	LARGE_INTEGER temp = currentTime;
	temp.QuadPart *= 1000000;
	double t = (double)(temp.QuadPart / frequency.QuadPart);
	return t;
}

float Frame::getTime_s()
{
	return (float)(getTime() / 1000000.0);
}

void Frame::end(SDL_Window *window)
{
	ran_frame();
	static int counter = 1;
	static double displayResult = 0.0;
	double result = (1.0 / (deltaTime2.QuadPart / frequency.QuadPart)) * 1000000.0;

	if (counter == 1)
	{
		displayResult = result;
		counter++;
	}
	else if (counter == 30)
	{
		displayResult = displayResult + ((result - displayResult) / (double)counter);
		std::stringstream stream;
		stream << std::fixed << std::setprecision(2) << displayResult;
		std::string s = stream.str();
		std::string FPS = "FPS: " + s;
		SDL_SetWindowTitle(window, FPS.c_str());
		//res.loadText("fps", util::va("FPS: %.0f", displayResult), 32, 0, 1080 - 32);
		counter = 1;
	}
	else
	{
		displayResult = displayResult + ((result - displayResult) / (double)counter);
		counter++;
	}
	
}

void Frame::end_f()
{
	frames++;
}