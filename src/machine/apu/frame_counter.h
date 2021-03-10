#pragma once

class PulseChannel;
class TriangleChannel;

class FrameCounter
{
public:
	bool mode = false;
	bool interrupt_inhibit = true;
	void ClockQuarter(PulseChannel *pulse, TriangleChannel *triangle);
	void ClockHalf(PulseChannel *pulse, TriangleChannel *triangle);
};