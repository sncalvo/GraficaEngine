#include "Settings.h"

namespace Engine
{
	Settings::Settings() : _isWireframe(false), _showTextures(true), _useInterpolation(true), _gameSpeed(GameSpeed::NORMAL)
	{
	}

	bool Settings::getIsWireframe() const
	{
		return _isWireframe;
	}

	void Settings::setIsWireframe(bool isWireframe)
	{
		// TODO: Change to wireframe mode if isWireframe == true
		_isWireframe = isWireframe;
	}

	bool Settings::getShowTextures() const
	{
		return _showTextures;
	}

	void Settings::setShowTextures(bool showTextures)
	{
		// TODO: Show or not textures depending on the value of `showTextures`
		_showTextures = showTextures;
	}

	bool Settings::getUseInterpolation() const
	{
		return _useInterpolation;
	}

	void Settings::setUseInterpolation(bool useInterpolation)
	{
		// TODO: Use interpolation or not depending on the value of `useInterpolation`
		_useInterpolation = useInterpolation;
	}

	GameSpeed Settings::getGameSpeed() const
	{
		return _gameSpeed;
	}

	void Settings::setGameSpeed(GameSpeed gameSpeed)
	{
		// TODO: Change game speed
		_gameSpeed = gameSpeed;
	}
}
