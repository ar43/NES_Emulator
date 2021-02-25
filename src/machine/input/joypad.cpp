#include "joypad.h"
#include "../../utility/utility.h"

uint8_t Joypad::Read()
{
	if (strobe)
	{
		return (state >> 1) & 1;
	}
	else if(index > 7)
	{
		return 1;
	}
	uint8_t ind = index;
	index++;
	return (state >> ind) & 1;
}

void Joypad::SetState(JoypadButtons button)
{
	utility::SetBit(&state, (size_t)button, true);
}

void Joypad::ResetState(JoypadButtons button)
{
	utility::SetBit(&state, (size_t)button, false);
}

void Joypad::Write(uint8_t value)
{
	if ((value & 1) == 1)
	{
		strobe = true;
		index = 0;
	}
	else
	{
		strobe = false;
	}
}