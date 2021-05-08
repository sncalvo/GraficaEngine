#pragma once

#include <vector>

#include "../Renderer/Model.h"
#include "../Renderer/Material.h"

#include "BaseGameObject.h"
#include "Scene.h"

namespace Engine
{
	class GameObject : public BaseGameObject
	{
	private:
		Model* _model;
		MaterialObject _material;
	public:
		GameObject(Model* model, MaterialObject material);
		GameObject(GameObject*);
		~GameObject();
		void draw() const;
		Model* getModel() const;
		MaterialObject getMaterial() const;
	};
}
