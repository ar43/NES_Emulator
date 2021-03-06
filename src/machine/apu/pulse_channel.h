#pragma once
#include <vector>
class PulseChannel
{
public:
	bool is_pulse1 = false;
	bool enable = false;
	int len = 0;
	int duty_cycle = 0;
	bool length_counter_halt = false;
	bool c = false;
	int envelope_divider = 0;
	int envelope_div = 0;
	int envelope = 15;
	int envelope_vol = 0;
	int timer_target = 0;
	int timer = 0;
	bool env_start = false;
	int _amp = 0;
	int freq = 0;
	int duty_index = 0;

	//sweep
	bool sweep_enable = false;
	int sweep_period = 0;
	int sweep_divider = 0;
	bool sweep_negate = false;
	int sweep_shift = 0;
	bool sweep_reload = false;
	bool muted_by_sweep = false;

	void ClockEnv();
	void ClockSL();
};