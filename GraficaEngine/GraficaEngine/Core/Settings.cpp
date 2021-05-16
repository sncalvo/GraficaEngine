#include "Settings.h"

namespace Engine
{
	Settings::Settings() : _isWireframe(false), _showTextures(true), _useInterpolation(true), _showColliders(false), _gameSpeed(GameSpeed::NORMAL)
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

	void Settings::setGameSpeed(GameSpeed gameSpeed)
	{
		// TODO: Change game speed
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
}
