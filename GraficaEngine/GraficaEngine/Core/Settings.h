#pragma once

#include <tuple>

#include <glm/glm.hpp>

#include "Light.h"

namespace Engine
{
	enum class GameSpeed
	{
		SLOW,
		NORMAL,
		FAST,
	};

	class Settings
	{
	private:
		DayTime _dayTime;
		bool _isWireframe, _showTextures, _useInterpolation, _showColliders;
		// Only applicable for perspective camera
		float _cameraNear, _cameraFar;
		GameSpeed _gameSpeed;
		Settings();

	public:
		static Settings &getInstance()
		{
			static Settings _instance;
			return _instance;
		}
		bool getIsWireframe() const;
		void setIsWireframe(bool);
		bool getShowTextures() const;
		void setShowTextures(bool);
		bool getUseInterpolation() const;
		void setUseInterpolation(bool);
		GameSpeed getGameSpeedEnum() const;
		float getGameSpeed() const;
		void setGameSpeed(GameSpeed);
		bool getShowColliders() const;
		void setShowColliders(bool);
		DayTime getDayTime() const;
		void setDayTime(DayTime);
		std::pair<float, float> getCameraNearAndFarPlane() const;
		std::vector<float> getShadowCascadeLevels() const;
	};
}
