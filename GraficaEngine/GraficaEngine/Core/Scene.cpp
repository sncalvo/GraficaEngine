#include "Scene.h"

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
		_gameObjects.push_back(gameObject);
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

	void Scene::removeGameObject(BaseGameObject *gameObject)
	{
		std::remove(_gameObjects.begin(), _gameObjects.end(), gameObject);
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
		for (BaseGameObject *baseGameObject : _gameObjects)
		{
			if (baseGameObject->isDrawable())
			{
				GameObject *gameObject = dynamic_cast<GameObject *>(baseGameObject);
				gameObject->draw();
			}
		}
	}

	void Scene::update()
	{
		_activeCamera->update();

		for (BaseGameObject *gameObject : _gameObjects)
		{
			gameObject->update();
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
