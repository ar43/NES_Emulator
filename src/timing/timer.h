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
private:
	bool repeat;
	double startTime;
	double tickTime;
	bool active;
};