#pragma once

#include <chrono>

class Time
{
private:
	static std::chrono::steady_clock::time_point _previousTime;
	static std::chrono::steady_clock::time_point _currentTime;

	static float _fixedTime;
	Time() {}

public:
	static void updateTime();
	static float getDeltaTime();
	static float getFixedTime();
};
