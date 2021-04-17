#pragma once

#include <SDL/SDL.h>

namespace Engine
{
	class Time
	{
	private:
		static Uint32 _previousTime;
		static Uint32 _currentTime;

		static float _fixedTime;
		Time() {}

	public:
		static void updateTime();
		static float getDeltaTime();
		static float getFixedTime();
	};
}