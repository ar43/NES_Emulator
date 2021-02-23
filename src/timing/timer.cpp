#include "timer.h"
#include "frame.h"

Timer::Timer()
{
	active = false;
}

void Timer::start(bool repeat, double tickTime, Frame* frame)
{
	this->repeat = repeat;
	double t = frame->getTime();
	startTime = t;
	this->tickTime = tickTime;
	active = true;
}

bool Timer::tick(Frame* frame)
{
	if (!active)
		return false;

	double t = frame->getTime();
	double dif = t - startTime;
	dif /= 1000.0;
	if (dif >= tickTime)
	{
		if (!repeat)
			active = false;
		else
			startTime = t;

		return true;
	}
	else
	{
		return false;
	}
}

void Timer::stop()
{
	this->active = false;
}

bool Timer::isActive()
{
	return this->active;
}