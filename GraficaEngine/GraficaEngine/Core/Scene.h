#pragma once

#include "GameObject.h"
#include "Camera.h"
#include "Light.h"

namespace Engine
{
	class GameObject;

	class Scene
	{
	private:
		Camera* _activeCamera;
		std::vector<GameObject*> _gameObjects;
		std::vector<Light*> _lights;
	public:
		Scene();
		void addLight(Light*);
		void removeLight(Light*);
		void addGameObject(GameObject*);
		void removeGameObject(GameObject*);
		Camera* getCamera();
		std::vector<Light*> getLights();
		void draw();
		void update();
		~Scene();
	};
}
