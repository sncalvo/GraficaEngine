#include "SceneManager.h"

namespace Engine
{
	SceneManager::SceneManager() :
		_activeScene(nullptr),
		_shouldRestart(false)
	{
	}

	void SceneManager::addScene(std::string sceneName, Scene *scene)
	{
		_scenes[sceneName] = scene;
	}

	void SceneManager::loadScene(std::string sceneName)
	{
		if (_activeScene != nullptr)
		{
			delete _activeScene;
		}

		_activeScene = new Scene(_scenes[sceneName]);
	}

	Scene *SceneManager::getActiveScene() const
	{
		return _activeScene;
	}

	void SceneManager::setShouldRestart(bool shouldRestart)
	{
		_shouldRestart = shouldRestart;
	}

	bool SceneManager::getShouldRestart() const
	{
		return _shouldRestart;
	}
}