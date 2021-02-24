#pragma once

class Frame;

class Timer
{
public:
	Timer();
	void start(bool repeat, double tickTime, Frame* frame);
	bool tick(Frame *frame);
	void stop();
	bool isActive();
	bool bypass = false;
private:
	bool repeat;
	double startTime;
	double tickTime;
	bool active;
	
};