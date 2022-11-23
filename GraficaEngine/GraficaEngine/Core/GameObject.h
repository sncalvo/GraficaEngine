#pragma once

#include <vector>

#include "../Renderer/Model.h"
#include "../Renderer/Material.h"

#include "BaseGameObject.h"
#include "Scene.h"
#include "Animator.h"

namespace Engine
{
	class GameObject : public BaseGameObject
	{
	private:
		Model *_model;
		MaterialObject _material;
		Collider *_collider;
		Animator *_animator;
	public:
		GameObject(Model *model, MaterialObject material);
		GameObject(Model *model, MaterialObject material, Animation *animation);
		GameObject(const GameObject *);
		GameObject *clone() const override;
		~GameObject();
		void setCollider(Collider *collider);
		Collider *getCollider() const;
		void draw(Shader *shader) const;
		Model *getModel() const;
		MaterialObject &getMaterial();
		std::tuple<std::vector<std::shared_ptr<MeshRenderer>>, std::vector<std::shared_ptr<ShadowRenderer>>, std::vector<std::shared_ptr<TextRenderer>>> getRenderers() override;
	};
}
