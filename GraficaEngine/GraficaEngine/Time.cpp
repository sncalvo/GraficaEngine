#include "Time.h"

#include <iostream>

float Time::_fixedTime = 0.05;
Uint32 Time::_previousTime;
Uint32 Time::_currentTime;

float Time::getDeltaTime()
{
	return float(_currentTime - _previousTime) / 1000.f;
}

float Time::getFixedTime()
{
	return _fixedTime;
}

void Time::updateTime()
{
	_previousTime = _currentTime;
	_currentTime = SDL_GetTicks();
}
