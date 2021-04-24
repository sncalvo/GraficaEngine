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

	void Scene::addLight(Light* light)
	{
		_lights.push_back(light);
	}

	void Scene::removeLight(Light* light)
	{
		std::remove(_lights.begin(), _lights.end(), light);
	}

	std::vector<Light*> Scene::getLights()
	{
		return _lights;
	}

	Camera* Scene::getCamera()
	{
		return _activeCamera;
	}

	void Scene::draw()
	{
		for (GameObject* gameObject : _gameObjects)
		{
			gameObject->draw();
		}
	}

	void Scene::update()
	{
		for (GameObject* gameObject : _gameObjects)
		{
			gameObject->update();
		}
	}

	Scene::~Scene()
	{
		delete _activeCamera;

		for (GameObject* gameObject : _gameObjects)
		{
			delete gameObject;
		}

		for (Light* light : _lights)
		{
			delete light;
		}

	}
}
