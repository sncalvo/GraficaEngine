#include "Scene.h"
#include <stdio.h>

#include <algorithm>

#include "Settings.h"

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

		if (gameObject->getCollider() != nullptr)
		{
			addCollider(gameObject->getCollider());
		}
	}

	void Scene::deleteGameObject(BaseGameObject *gameObject)
	{
		_gameObjectsToDelete.insert(gameObject);
	}

	BaseGameObject *Scene::getGameObjectWithTag(std::string tag)
	{
		for (BaseGameObject *gameObject : _gameObjects)
		{
			if (gameObject->hasTag(tag))
			{
				return gameObject;
			}
			BaseGameObject* child = gameObject->getChildWithTag(tag);
			if (child != nullptr)
			{
				return child;
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

		if (Settings::getInstance().getShowColliders())
		{
			for (Collider *collider : _colliders)
			{
				collider->draw();
			}
		}
	}

	void Scene::start()
	{
		for (BaseGameObject* gameObject : _gameObjects)
		{
			gameObject->start();
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

	void Scene::addCollider(Collider *collider)
	{
		_colliders.push_back(collider);
	}

	void Scene::removeCollider(Collider *collider)
	{
		if (collider == nullptr)
		{
			return;
		}

		_colliders.erase(
			std::remove(
				_colliders.begin(),
				_colliders.end(),
				collider),
			_colliders.end());
	}

	void Scene::flushQueuedGameObjects()
	{
		_gameObjects.insert(
			_gameObjects.end(),
			_queuedGameObjects.begin(),
			_queuedGameObjects.end());

		for (BaseGameObject* gameObject : _queuedGameObjects)
		{
			gameObject->start();
		}

		_queuedGameObjects.clear();

		for (BaseGameObject *gameObjectToDelete : _gameObjectsToDelete)
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
						gameObjectToDelete),
					_gameObjects.end());

				delete gameObjectToDelete;
			}
		}

		_gameObjectsToDelete.clear();
	}

	void Scene::physicsUpdate()
	{
		for (Collider *collider : _colliders)
		{
			collider->resetCollisions();
		}

		for (Collider *first : _colliders)
		{
			for (Collider *second : _colliders)
			{
				if (!Collider::intersect(first, second))
				{
					continue;
				}

				first->collide(second);
				second->collide(first);
			}
		}
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
