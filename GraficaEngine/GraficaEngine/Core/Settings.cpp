#include "Settings.h"

constexpr float CAMERA_NEAR_DEFAULT = 0.1f;
constexpr float CAMERA_FAR_DEFAULT = 500.f;
constexpr float EXPOSURE_DEFAULT = 2.f;
constexpr unsigned int SHADOW_RESOLUTION_DEFAULT = 2048;
constexpr unsigned int DEFAULT_WIDTH = 1080;
constexpr unsigned int DEFAULT_HEIGHT = 800;
constexpr bool DEFAULT_FULLSCREEN = false;
constexpr float FOG_MAX_DIST = 60.f;
constexpr float FOG_MIN_DIST = 0.1f;
constexpr glm::vec3 FOG_COLOR = glm::vec3(0.4f);
constexpr float DEFAULT_BIAS_MODIFIER = 1.f;

namespace Engine
{
	Settings::Settings() : _isWireframe(false),
						   _showTextures(true),
						   _useInterpolation(true),
						   _showColliders(false),
						   _gameSpeed(GameSpeed::NORMAL),
						   _dayTime(DayTime::MIDDAY),
						   _cameraNear(CAMERA_NEAR_DEFAULT), _cameraFar(CAMERA_FAR_DEFAULT),
						   _exposure(EXPOSURE_DEFAULT), _shadowResolution(SHADOW_RESOLUTION_DEFAULT),
						   _windowWidth(DEFAULT_WIDTH), _windowHeight(DEFAULT_HEIGHT), _fullscreen(DEFAULT_FULLSCREEN),
						   _fogMaxDist(FOG_MAX_DIST), _fogMinDist(FOG_MIN_DIST), _fogColor(FOG_COLOR), _biasModifier(DEFAULT_BIAS_MODIFIER)
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

	std::pair<unsigned int, unsigned int> Settings::getWindowSize() const
	{
		return { _windowWidth, _windowHeight };
	}

	void Settings::setWindowSize(unsigned int width, unsigned int height)
	{
		_windowWidth = width;
		_windowHeight = height;
	}

	bool Settings::getFullscreen() const
	{
		return _fullscreen;
	}

	void Settings::setFullscreen(bool fullscreen)
	{
		_fullscreen = fullscreen;
	}

	bool Settings::getDebugShadowMap() const
	{
		return _debugShadowMap;
	}

	void Settings::setDebugShadowMap(bool debugShadowMap)
	{
		_debugShadowMap = debugShadowMap;
	}

	std::pair<float, float> Settings::getFogRange() const
	{
		return { _fogMinDist, _fogMaxDist };
	}

	void Settings::setFog(float min, float max)
	{
		_fogMinDist = min;
		_fogMaxDist = max;
	}

	glm::vec3 Settings::getFogColor() const
	{
		return _fogColor;
	}

	void Settings::setFogColor(glm::vec3 color)
	{
		_fogColor = color;
	}

	bool Settings::getBloom() const
	{
		return _bloom;
	}

	void Settings::setBloom(bool bloom)
	{
		_bloom = bloom;
	}

	bool Settings::getSnow() const
	{
		return _snow;
	}

	void Settings::setSnow(bool snow)
	{
		_snow = snow;
	}

	bool Settings::getFrustumCulling() const
	{
		return _frustum_culling;
	}

	void Settings::setFrustumCulling(bool frustum_culling)
	{
		_frustum_culling = frustum_culling;
	}

	int Settings::getFrustumCullingCamera() const
	{
		return _frustum_culling_camera;
	}

	void Settings::setFrustumCullingCamera(int frustum_culling_camera)
	{
		_frustum_culling_camera = frustum_culling_camera;
	}

	bool Settings::getFog() const
	{
		return _fog;
	}

	void Settings::setFog(bool fog)
	{
		_fog = fog;
	}

	float Settings::getBiasModifier() const
	{
		return _biasModifier;
	}

	void Settings::setBiasModifier(float bias)
	{
		_biasModifier = bias;
	}
}
