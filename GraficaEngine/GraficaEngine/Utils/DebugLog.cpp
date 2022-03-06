#include "DebugLog.h"

#include <iostream>
#include <sstream>

namespace Engine
{
	bool DebugLog::_isDebugMode = true;

	std::string bold_red = "\033[1;31m";
	std::string red = "\033[0;31m";
	std::string bold_cyan = "\033[1;36m";
	std::string cyan = "\033[0;36m";
	std::string bold_green = "\033[1;32m";
	std::string green = "\033[0;32m";
	std::string bold_yellow = "\033[1;33m";
	std::string yellow = "\033[0;33m";
	std::string reset_color = "\033[0m";
	std::string clear_screen = "[H[2J";
	std::string clear_line = "\r\e[0K";

	std::string errorPrefix = "Error:   ";
	std::string warningPrefix = "Warning: ";
	std::string debugPrefix = "Debug:   ";

	void DebugLog::error(std::string msg)
	{
		std::cerr << bold_red << errorPrefix << msg << std::endl;
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

	void DebugLog::debug(glm::vec3 vector)
	{
		if (_isDebugMode)
			std::cerr << bold_cyan << debugPrefix << "X: " << vector.x << ", Y: " << vector.y << ", Z: " << vector.z << reset_color << std::endl;
	}

	void DebugLog::debug(glm::highp_mat4 matrix)
	{
		if (_isDebugMode)
		{
			std::cerr << bold_cyan << debugPrefix;
			std::cerr << "Begin matrix" << std::endl;
			std::cerr << "X: " << matrix[0].x << ", Y: " << matrix[0].y << ", Z: " << matrix[0].z << ", W: " << matrix[0].w << std::endl;
			std::cerr << "X: " << matrix[1].x << ", Y: " << matrix[1].y << ", Z: " << matrix[1].z << ", W: " << matrix[1].w << std::endl;
			std::cerr << "X: " << matrix[2].x << ", Y: " << matrix[2].y << ", Z: " << matrix[2].z << ", W: " << matrix[2].w << std::endl;
			std::cerr << "X: " << matrix[3].x << ", Y: " << matrix[3].y << ", Z: " << matrix[3].z << ", W: " << matrix[3].w << std::endl;
			std::cerr << reset_color << std::endl;
		}
	}

	void DebugLog::debug(float number)
	{
		if (_isDebugMode)
		{
			std::cerr << bold_cyan << debugPrefix << number << reset_color << std::endl;
		}
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
}
