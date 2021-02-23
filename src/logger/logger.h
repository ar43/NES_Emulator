#pragma once
#include <string>
#include <vector>

namespace logger
{
	extern bool CPU_TEST_MODE;
	extern std::vector<std::string> cpu_test_buffer;

	enum class LogType
	{
		INFO,
		DEBUG,
		WARNING,
		FATAL_ERROR,
		INTERNAL_ERROR
	};

	void PrintLine(LogType logType, std::string msg);
	void Print(LogType logType, std::string msg);
	void WriteToFile(std::string msg);
	void WriteTestToFile();
}