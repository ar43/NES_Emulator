#pragma once
class TriangleChannel
{
public:
	bool enable = false;
	int length_counter = 0;
	int linear_counter = 0;
	bool linear_control = false;
	bool length_enabled = false;
	int linear_load = 0;
	int freq_timer = 0;
	
	int output = 0;
	bool linear_reload = false;
	void Clock();
	void ClockQuarter();
	void ClockHalf();
private:
	int tri_step = 0;
	int freq_counter = 0;
};