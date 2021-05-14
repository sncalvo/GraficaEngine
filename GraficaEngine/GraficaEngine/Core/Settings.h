#pragma once

namespace Engine
{
	enum class GameSpeed {
		SLOW,
		NORMAL,
		FAST,
	};

	class Settings
	{
	private:
		bool _isWireframe, _showTextures, _useInterpolation;
		GameSpeed _gameSpeed;
		Settings();

	public:
		static Settings& getInstance()
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
		float getGameSpeed() const;
		void setGameSpeed(GameSpeed);
	};
}
