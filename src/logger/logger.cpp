#include "logger.h"
#include <iostream>
#include <csignal>
#include <chrono>
#include <ctime>


namespace logger
{
	void WriteToFile(std::string msg)
	{
		if (CPU_TEST_MODE)
		{
			//...
		}
	}

	std::string TypeToString(LogType logType)
	{
		switch (logType)
		{
		case LogType::DEBUG:
			return "(DEBUG)";
		case LogType::INFO:
			return "(INFO)";
		case LogType::WARNING:
			return "(WARNING)";
		case LogType::ERROR:
			return "(ERROR)";
		default:
			return "(UNKNOWN)";
		}
	}

	std::string GetTimetamp()
	{
		std::time_t t = std::time(0);   // get time now
		std::tm now{};
		localtime_s(&now,&t);
		return "[" + std::to_string(now.tm_hour) + ":" + std::to_string(now.tm_min) + ":" + std::to_string(now.tm_sec) + "]";
	}

	void PrintLine(LogType logType, std::string msg)
	{
		std::cout << GetTimetamp() << " " << TypeToString(logType) << ": " << msg << std::endl;
#ifdef _DEBUG
		if (logType == LogType::ERROR)
			std::raise(SIGINT);
#endif
	}

	void Print(LogType logType, std::string msg)
	{
		std::cout << GetTimetamp() << " " << TypeToString(logType) << ": " << msg;
#ifdef _DEBUG
		if (logType == LogType::ERROR)
			std::raise(SIGINT);
#endif
	}
}

