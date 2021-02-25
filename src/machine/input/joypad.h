#pragma once
#include <cstdint>

enum class JoypadButtons
{
	A,
	B,
	SELECT,
	START,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Joypad
{
public:
	void Write(uint8_t value);
	uint8_t Read();
	void SetState(JoypadButtons button);
	void ResetState(JoypadButtons button);
private:
	uint8_t state = 0;
	uint8_t index = 0;
	bool strobe = false;
};