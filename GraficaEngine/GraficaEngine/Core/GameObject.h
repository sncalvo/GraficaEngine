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
		Model *_model;
		MaterialObject _material;
		Collider *_collider;

	public:
		GameObject(Model *model, MaterialObject material);
		GameObject(const GameObject *);
		GameObject *clone() const override;
		~GameObject();
		void setCollider(Collider *collider);
		Collider *getCollider() const;
		void draw() const;
		Model *getModel() const;
		MaterialObject &getMaterial();
	};
}
