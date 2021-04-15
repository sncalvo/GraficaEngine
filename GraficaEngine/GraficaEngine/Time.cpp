#include "Time.h"

float Time::_fixedTime = 0.05;
Uint32 Time::_previousTime = 0;
Uint32 Time::_currentTime = 0;

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
