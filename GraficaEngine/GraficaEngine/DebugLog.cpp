#include "DebugLog.h"

#include <iostream>
#include <sstream>

bool DebugLog::_isDebugMode = false;

std::string bold_red = "\e[1;31m";
std::string red = "\e[0;31m";
std::string bold_cyan = "\e[1;36m";
std::string cyan = "\e[0;36m";
std::string bold_green = "\e[1;32m";
std::string green = "\e[0;32m";
std::string bold_yellow = "\e[1;33m";
std::string yellow = "\e[0;33m";
std::string reset_color = "\e[0m";
std::string clear_screen = "[H[2J";
std::string clear_line = "\r\e[0K";

std::string errorPrefix = "Error:   ";
std::string warningPrefix = "Warning: ";
std::string debugPrefix = "Debug:   ";

void DebugLog::error(std::string msg)
{
	std::cerr << bold_red << errorPrefix << msg << reset_color << std::endl;
}

void DebugLog::warning(std::string msg)
{
	std::cerr << bold_yellow << warningPrefix << msg << reset_color << std::endl;
}

void DebugLog::log(std::string msg)
{
	std::cout << bold_green << msg << reset_color << std::endl;
}

void DebugLog::log_raw(std::string msg)
{
	std::cout << bold_green << msg << reset_color;
}

void DebugLog::debug(std::string msg)
{
	if (_isDebugMode)
		std::cerr << bold_cyan << debugPrefix << msg << reset_color << std::endl;
}

void DebugLog::debugMode(bool option)
{
	if (option == false)
	{
		_isDebugMode = false;
		return;
	}

	_isDebugMode = true;
	DebugLog::debug(">Debug mode activated");
}

void DebugLog::clearLine()
{
	std::cout << clear_line;
}

std::string DebugLog::intToString(int num)
{
	std::stringstream ss;
	ss.str("");
	ss << num;

	std::string numStr = ss.str();
	return numStr;
}
