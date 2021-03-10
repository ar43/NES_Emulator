#pragma once

class PulseChannel;
class TriangleChannel;
class NoiseChannel;

class FrameCounter
{
public:
	bool mode = false;
	bool interrupt_inhibit = true;
	void ClockQuarter(PulseChannel *pulse, TriangleChannel *triangle, NoiseChannel *noise);
	void ClockHalf(PulseChannel *pulse, TriangleChannel *triangle, NoiseChannel *noise);
};