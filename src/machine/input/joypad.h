#pragma once
#include <cstdint>
#include <string>

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
	static const int num_keys = 8;

	void Write(uint8_t value);
	uint8_t Read();
	void SetState(JoypadButtons button);
	void ResetState(JoypadButtons button);
	void Clear();

	static std::string ButtonToString(JoypadButtons button);
private:
	uint8_t state = 0;
	uint8_t index = 0;
	bool strobe = false;
};