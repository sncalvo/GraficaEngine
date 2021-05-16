#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>

#include "GameObject.h"
#include "Camera.h"
#include "Light.h"
#include "TextObject.h"

#include "../Physics/Collider.h"

namespace Engine
{
	class Scene
	{
	private:
		Camera *_activeCamera;
		std::map<std::string, Camera *> _cameras;
		std::vector<std::string> _cameraNames;
		std::vector<BaseGameObject *> _gameObjects;
		std::vector<BaseGameObject *> _queuedGameObjects;
		std::set<BaseGameObject *> _gameObjectsToDelete;
		std::vector<Collider *> _colliders;
		std::vector<Light *> _lights;

	public:
		Scene(Camera *);
		void addLight(Light *);
		void removeLight(Light *);
		void addGameObject(BaseGameObject *);
		void addCollider(Collider *);
		void removeCollider(Collider *);
		void deleteGameObject(BaseGameObject *);
		void flushQueuedGameObjects();
		BaseGameObject *getGameObjectWithTag(std::string);
		void addCamera(std::string, Camera *);
		void setActiveCamera(std::string);
		Camera *getActiveCamera();
		Camera *getCamera(std::string);
		std::vector<std::string> getCameraNames() const;
		std::vector<Light *> getLights();
		void draw();
		void start();
		void update();
		void physicsUpdate();
		~Scene();
	};
}
