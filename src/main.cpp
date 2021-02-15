#include "logger/logger.h"



int main()
{
	logger::PrintLine(logger::LogType::INFO, "Starting NES emulator");

	return 0;
}