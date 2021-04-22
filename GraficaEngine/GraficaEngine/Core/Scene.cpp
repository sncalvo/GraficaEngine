#include "Scene.h"

namespace Engine
{
	Scene::Scene()
	{
		_activeCamera = new Camera();
	}

	void Scene::addGameObject(GameObject* gameObject)
	{
		gameObject->setScene(this);
		_gameObjects.push_back(gameObject);
	}

	void Scene::removeGameObject(GameObject* gameObject)
	{
		std::remove(_gameObjects.begin(), _gameObjects.end(), gameObject);
	}

	Camera* Scene::getCamera()
	{
		return _activeCamera;
	}

	void Scene::draw()
	{
		for (GameObject* gameObject : _gameObjects)
		{
			gameObject->update();
		}
	}

	void Scene::update()
	{
		for (GameObject* gameObject : _gameObjects)
		{
			gameObject->draw();
		}
	}

	Scene::~Scene()
	{
		delete _activeCamera;

		for (GameObject* gameObject : _gameObjects)
		{
			delete gameObject;
		}
	}
}
