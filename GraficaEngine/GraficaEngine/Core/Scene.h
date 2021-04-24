#pragma once

#include "GameObject.h"
#include "Camera.h"

namespace Engine
{
	class GameObject;

	class Scene
	{
	private:
		Camera* _activeCamera;
		std::vector<GameObject*> _gameObjects;
	public:
		Scene();
		void addGameObject(GameObject*);
		void removeGameObject(GameObject*);
		Camera* getCamera();
		void draw();
		void update();
		~Scene();
	};
}
