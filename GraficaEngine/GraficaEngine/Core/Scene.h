#pragma once

#include <string>
#include <vector>
#include <map>

#include "GameObject.h"
#include "Camera.h"
#include "Light.h"
#include "TextObject.h"

namespace Engine
{
	class Scene
	{
	private:
		Camera* _activeCamera;
		std::map<std::string, Camera*> _cameras;
		std::vector<std::string> _cameraNames;
		std::vector<BaseGameObject*> _gameObjects;
		std::vector<BaseGameObject*> _queuedGameObjects;
		std::vector<BaseGameObject*> _gameObjectsToDelete;
		std::vector<Light*> _lights;
	public:
		Scene(Camera*);
		void addLight(Light*);
		void removeLight(Light*);
		void addGameObject(BaseGameObject*);
		void deleteGameObject(BaseGameObject*);
		void flushQueuedGameObjects();
		BaseGameObject* getGameObjectWithTag(std::string);
		void addCamera(std::string, Camera*);
		void setActiveCamera(std::string);
		Camera* getActiveCamera();
		Camera* getCamera(std::string);
		std::vector<std::string> getCameraNames() const;
		std::vector<Light*> getLights();
		void draw();
		void update();
		~Scene();
	};
}
