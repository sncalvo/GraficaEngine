#include "Time.h"

#include <iostream>

float Time::_fixedTime = 0.05;
std::chrono::steady_clock::time_point Time::_previousTime;
std::chrono::steady_clock::time_point Time::_currentTime;

float Time::getDeltaTime()
{
	// Such a mouthfull
	return std::chrono::duration_cast<std::chrono::microseconds>(_currentTime - _previousTime).count() / 1000000.0;
}

float Time::getFixedTime()
{
	return _fixedTime;
}

void Time::updateTime()
{
	_previousTime = _currentTime;
	_currentTime = std::chrono::high_resolution_clock::now();
}
