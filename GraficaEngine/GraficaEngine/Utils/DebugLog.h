#pragma once

#include <string>

namespace Engine
{
	class DebugLog
	{
	private:
		static bool _isDebugMode;
		static bool _isVerboseMode;

	public:
		static void error(std::string msg);
		static void warning(std::string msg);
		static void log(std::string msg);
		static void log_raw(std::string msg);
		static void debug(std::string msg);
		static void debugMode(bool option);
		static void clearLine();

		static std::string intToString(int num);
	};
}
