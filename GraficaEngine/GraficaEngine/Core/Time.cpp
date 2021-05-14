#include "Time.h"

#include "Settings.h"

namespace Engine
{
	float Time::_fixedTime = 0.05;
	Uint32 Time::_previousTime = 0;
	Uint32 Time::_currentTime = 0;

	float Time::getDeltaTime()
	{
		Settings& settings = Settings::getInstance();
		return (float(_currentTime - _previousTime) / 1000.f) * settings.getGameSpeed();
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

	float Time::getTime()
	{
		return SDL_GetTicks() / 1000.f;
	}
}
