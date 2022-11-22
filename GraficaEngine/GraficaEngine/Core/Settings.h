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
		float _exposure;
		unsigned int _shadowResolution;
		unsigned int _windowWidth;
		unsigned int _windowHeight;
		bool _fullscreen;
		float _fogMaxDist;
		float _fogMinDist;
		glm::vec3 _fogColor;

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
		float getExposure() const;
		void setExposure(float);
		unsigned int getShadowResolution() const;
		void setShadowResolution(unsigned int);
		std::pair<unsigned int, unsigned int> getWindowSize() const;
		void setWindowSize(unsigned int width, unsigned int height);
		std::pair<float, float> getCameraNearAndFarPlane() const;
		std::vector<float> getShadowCascadeLevels() const;
		bool getFullscreen() const;
		void setFullscreen(bool);
		std::pair<float, float> getFogRange() const;
		void setFog(float min, float max);
		glm::vec3 getFogColor() const;
		void setFogColor(glm::vec3 color);
	};
}
