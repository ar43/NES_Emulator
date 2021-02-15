#pragma once
#include <string>

namespace logger
{
	enum class LogType
	{
		INFO,
		DEBUG,
		WARNING,
		ERROR
	};

	void PrintLine(LogType logType, std::string msg);
	void Print(LogType logType, std::string msg);
}