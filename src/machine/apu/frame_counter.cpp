#include "frame_counter.h"
#include "pulse_channel.h"
#include "triangle_channel.h"

void FrameCounter::ClockQuarter(PulseChannel *pulse, TriangleChannel *triangle)
{
	pulse[0].ClockQuarter();
	pulse[1].ClockQuarter();
	triangle->ClockQuarter();
}

void FrameCounter::ClockHalf(PulseChannel *pulse, TriangleChannel *triangle)
{
	pulse[0].ClockHalf();
	pulse[1].ClockHalf();
	triangle->ClockHalf();
}