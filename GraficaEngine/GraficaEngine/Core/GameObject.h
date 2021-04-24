#pragma once

#include <vector>

#include "../Renderer/Model.h"
#include "../Renderer/Material.h"

#include "Scene.h"
#include "Transform.h"
#include "Behaviour.h"

namespace Engine
{
	class Scene;

	class GameObject
	{
	private:
		Model _model;
		MaterialObject _material;
		std::vector<Behaviour*> _behaviours;
		Scene* _scene;
		// GameObject[] _children;
	public:
		GameObject(Model model, MaterialObject material);
		~GameObject();
		Transform transform;
		void setScene(Scene*);
		Scene* getScene();
		void update();
		void draw() const;
		void addBehaviour(Behaviour*);
	};
}
