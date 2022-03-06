#pragma once

#include <map>
#include <string>

#include "Scene.h"

namespace Engine
{
	class SceneManager
	{
	private:
		SceneManager();
		Scene *_activeScene;
		std::map<std::string, Scene*> _scenes;
		bool _shouldRestart;
	public:
		static SceneManager& getInstance()
		{
			static SceneManager _instance;
			return _instance;
		}
		Scene *getActiveScene() const;
		void loadScene(std::string);
		void addScene(std::string sceneName, Scene *scene);
		void setShouldRestart(bool shouldRestart);
		bool getShouldRestart() const;
	};
}