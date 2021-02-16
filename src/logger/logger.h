#pragma once
#include <string>

namespace logger
{
	constexpr bool CPU_TEST_MODE = true;

	enum class LogType
	{
		INFO,
		DEBUG,
		WARNING,
		ERROR
	};

	void PrintLine(LogType logType, std::string msg);
	void Print(LogType logType, std::string msg);
	void WriteToFile(std::string msg);
}