#pragma once

#include <string>

#include <glm/glm.hpp>

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
		static void debug(glm::vec3 vector);
		static void debug(glm::highp_mat4 matrix);
		static void debug(float number);
		static void debugMode(bool option);
		static void clearLine();

		static std::string intToString(int num);
	};
}
