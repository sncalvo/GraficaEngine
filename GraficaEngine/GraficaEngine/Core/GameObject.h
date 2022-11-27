#pragma once

#include <vector>

#include "../Renderer/Model.h"
#include "../Renderer/Material.h"
#include "../Physics/PhysicsManager.h"

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
		Animator *_animator;
		btRigidBody* _rigidBody;
		glm::vec3 _rigidBodyCenterOffset;
		Aabb* _aabb;
	public:
		GameObject(Model *model, MaterialObject material);
		GameObject(Model *model, MaterialObject material, Animation *animation);
		GameObject(const GameObject *);
		GameObject *clone() const override;
		~GameObject();
		btRigidBody* getRigidBody() const;
		void setRigidBody(btRigidBody*);
		void syncTransformWithRigidBody() override;
		void draw(Shader *shader) const;
		Model *getModel() const;
		MaterialObject &getMaterial();
		void calculateAabb();
		Aabb* getAabb() const;
		void setRigidBodyCenterOffset(glm::vec3 offset);
		std::tuple<std::vector<std::shared_ptr<MeshRenderer>>, std::vector<std::shared_ptr<ShadowRenderer>>, std::vector<std::shared_ptr<TextRenderer>>> getRenderers() override;
	};
}
