#include "Settings.h"

constexpr float CAMERA_NEAR_DEFAULT = 0.1f;
constexpr float CAMERA_FAR_DEFAULT = 50.f;
constexpr float EXPOSURE_DEFAULT = 2.f;
constexpr unsigned int SHADOW_RESOLUTION_DEFAULT = 2048;

namespace Engine
{
	Settings::Settings() : _isWireframe(false),
						   _showTextures(true),
						   _useInterpolation(false),
						   _showColliders(false),
						   _gameSpeed(GameSpeed::NORMAL),
						   _dayTime(DayTime::MIDDAY),
						   _cameraNear(CAMERA_NEAR_DEFAULT), _cameraFar(CAMERA_FAR_DEFAULT),
						   _exposure(EXPOSURE_DEFAULT), _shadowResolution(SHADOW_RESOLUTION_DEFAULT)
	{
	}

	bool Settings::getIsWireframe() const
	{
		return _isWireframe;
	}

	void Settings::setIsWireframe(bool isWireframe)
	{
		_isWireframe = isWireframe;
	}

	bool Settings::getShowTextures() const
	{
		return _showTextures;
	}

	void Settings::setShowTextures(bool showTextures)
	{
		_showTextures = showTextures;
	}

	bool Settings::getUseInterpolation() const
	{
		return _useInterpolation;
	}

	void Settings::setUseInterpolation(bool useInterpolation)
	{
		_useInterpolation = useInterpolation;
	}

	float Settings::getGameSpeed() const
	{
		switch (_gameSpeed)
		{
		case GameSpeed::SLOW:
			return .5f;
			break;
		case GameSpeed::NORMAL:
			return 1.f;
			break;
		case GameSpeed::FAST:
			return 2.f;
			break;
		}
	}

	GameSpeed Settings::getGameSpeedEnum() const
	{
		return _gameSpeed;
	}

	void Settings::setGameSpeed(GameSpeed gameSpeed)
	{
		_gameSpeed = gameSpeed;
	}

	bool Settings::getShowColliders() const
	{
		return _showColliders;
	}

	void Settings::setShowColliders(bool showColliders)
	{
		_showColliders = showColliders;
	}

	DayTime Settings::getDayTime() const
	{
		return _dayTime;
	}

	void Settings::setDayTime(DayTime dayTime)
	{
		_dayTime = dayTime;
	}

	std::pair<float, float> Settings::getCameraNearAndFarPlane() const
	{
		return { _cameraNear, _cameraFar };
	}

	std::vector<float> Settings::getShadowCascadeLevels() const
	{
		std::vector<float> shadowCascadeLevels{ _cameraFar / 50.0f, _cameraFar / 25.0f, _cameraFar / 10.0f, _cameraFar / 2.0f };

		return shadowCascadeLevels;
	}

	float Settings::getExposure() const
	{
		return _exposure;
	}

	void Settings::setExposure(float exposure)
	{
		_exposure = exposure;
	}

	unsigned int Settings::getShadowResolution() const
	{
		return _shadowResolution;
	}

	void Settings::setShadowResolution(unsigned int resolution)
	{
		_shadowResolution = resolution;
	}
}
