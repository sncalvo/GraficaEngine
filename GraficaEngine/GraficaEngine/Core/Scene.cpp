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

	Scene::Scene(const Scene *otherScene)
	{
		for (BaseGameObject *gameObject : otherScene->_gameObjects)
		{
			BaseGameObject *gameObjectCopy = gameObject->clone();
			addGameObject(gameObjectCopy);
		}

		for (BaseGameObject *gameObject : otherScene->_queuedGameObjects)
		{
			BaseGameObject *gameObjectCopy = gameObject->clone();
			addGameObject(gameObjectCopy);
		}

		for (std::string cameraName : otherScene->_cameraNames)
		{
			Camera* cameraClone = otherScene->_cameras.at(cameraName)->clone();
			addCamera(cameraName, cameraClone);
		}

		setSkybox(otherScene->_skybox);

		setActiveCamera("default"); // TODO: Make generic
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

	std::vector<Light *> Scene::getLights()
	{
		Light *light = Light::lightPresets[Settings::getInstance().getDayTime()];
		return std::vector<Light *>{light};
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

	void Scene::draw(Shader *shader)
	{
		for (BaseGameObject *gameObject : _gameObjects)
		{
			gameObject->draw(shader);
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

/* 		for (BaseGameObject *gameObject : _gameObjects)
		{
			gameObject->update();
		} */
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

	void Scene::addRenderers(BaseGameObject* gameObject)
	{
		auto [meshRenderers, shadowRenderers, textRenderers] = gameObject->getRenderers();
		_meshRenderers[gameObject->uuid] = meshRenderers;
		_shadowRenderers[gameObject->uuid] = shadowRenderers;
		_textRenderers[gameObject->uuid] = textRenderers;
	}

	void Scene::removeRenderers(BaseGameObject* gameObject)
	{
		_meshRenderers.erase(gameObject->uuid);
		_shadowRenderers.erase(gameObject->uuid);
		_textRenderers.erase(gameObject->uuid);
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
			addRenderers(gameObject);
		}

		_queuedGameObjects.clear();

		for (BaseGameObject *gameObjectToDelete : _gameObjectsToDelete)
		{
			removeRenderers(gameObjectToDelete);

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

	void Scene::animationsUpdate(float deltaTime) {
		for (auto& [key, value] : _meshRenderers) {
			for (auto renderer : value) {
				renderer->updateAnimation(deltaTime);
			}
		}
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
	}

	void Scene::drawSkybox(glm::mat4 projection, glm::mat4 view)
	{
		if (_skybox != nullptr)
		{
			_skybox->draw(projection, view, *Light::lightPresets[Settings::getInstance().getDayTime()]);
		}
	}

	void Scene::setParticleSystem(ParticleSystem *ps)
	{
		_ps = ps;
	}

	void Scene::drawParticles()
	{
		if (_ps != nullptr)
		{
			_ps->draw(getActiveCamera());
		}
	}

	void Scene::setSkybox(Skybox *skybox)
	{
		_skybox = skybox;
	}
}
