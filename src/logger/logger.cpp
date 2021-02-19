#include "logger.h"
#include <iostream>
#include <csignal>
#include <chrono>
#include <ctime>
#include <fstream>


namespace logger
{
	std::vector<std::string> cpu_test_buffer;

	void WriteToFile(std::string msg)
	{
		PrintLine(LogType::INFO, "WriteToFile not implemented");
	}

	void WriteTestToFile()
	{
		std::ofstream out("cpu_testing/cpu_test.log");
		for (auto& str : cpu_test_buffer)
		{
			out << str;
		}
		out.close();
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

