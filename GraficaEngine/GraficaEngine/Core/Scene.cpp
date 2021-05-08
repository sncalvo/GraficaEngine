#include "Scene.h"

#include <algorithm>

namespace Engine
{
	Scene::Scene(Camera *defaultCamera)
	{
		_cameraNames.push_back("default");
		_cameras["default"] = defaultCamera;
		_activeCamera = defaultCamera;
	}

	void Scene::addGameObject(BaseGameObject *gameObject)
	{
		gameObject->setScene(this);
		_queuedGameObjects.push_back(gameObject);
	}

	void Scene::deleteGameObject(BaseGameObject* gameObject)
	{
		_gameObjectsToDelete.push_back(gameObject);
	}

	BaseGameObject *Scene::getGameObjectWithTag(std::string tag)
	{
		for (BaseGameObject *gameObject : _gameObjects)
		{
			if (gameObject->hasTag(tag))
			{
				return gameObject;
			}
		}
		return nullptr;
	}

	void Scene::addLight(Light *light)
	{
		_lights.push_back(light);
	}

	void Scene::removeLight(Light *light)
	{
		std::remove(_lights.begin(), _lights.end(), light);
	}

	std::vector<Light *> Scene::getLights()
	{
		return _lights;
	}

	Camera *Scene::getActiveCamera()
	{
		return _activeCamera;
	}

	Camera *Scene::getCamera(std::string cameraName)
	{
		return _cameras[cameraName];
	}

	void Scene::addCamera(std::string cameraName, Camera *camera)
	{
		camera->setScene(this);
		_cameraNames.push_back(cameraName);
		_cameras[cameraName] = camera;
	}

	void Scene::setActiveCamera(std::string cameraName)
	{
		_activeCamera = getCamera(cameraName);
	}

	std::vector<std::string> Scene::getCameraNames() const
	{
		return _cameraNames;
	}

	void Scene::draw()
	{
		for (BaseGameObject *gameObject : _gameObjects)
		{
			gameObject->draw();
		}
	}

	void Scene::update()
	{
		_activeCamera->update();

		for (BaseGameObject *gameObject : _gameObjects)
		{
			gameObject->update();
		}

		flushQueuedGameObjects();
	}

	void Scene::flushQueuedGameObjects()
	{
		_gameObjects.insert(
			_gameObjects.end(),
			_queuedGameObjects.begin(),
			_queuedGameObjects.end()
		);

		_queuedGameObjects.clear();

		for (BaseGameObject* gameObjectToDelete : _gameObjectsToDelete)
		{
			if (gameObjectToDelete->hasParent())
			{
				gameObjectToDelete->getParent()->deleteChild(gameObjectToDelete);
			}
			else
			{
				_gameObjects.erase(
					std::remove(
						_gameObjects.begin(),
						_gameObjects.end(),
						gameObjectToDelete
					),
					_gameObjects.end()
				);

				delete gameObjectToDelete;
			}
		}

		_gameObjectsToDelete.clear();
	}

	Scene::~Scene()
	{
		for (std::pair<std::string, Camera *> entry : _cameras)
		{
			delete entry.second;
		}

		for (BaseGameObject *gameObject : _gameObjects)
		{
			delete gameObject;
		}

		for (Light *light : _lights)
		{
			delete light;
		}
	}
}
