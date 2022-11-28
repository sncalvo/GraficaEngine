#include "GameObject.h"

namespace Engine
{
	GameObject::GameObject(Model* model, MaterialObject material) : _model(model), _material(material), _rigidBody(nullptr), _rigidBodyCenterOffset(glm::vec3(0.f))
	{
		for (auto renderer : model->getMeshRenderers())
		{
			renderer->setTransform(&transform);
		}
		for (auto renderer : model->getShadowRenderers())
		{
			renderer->setTransform(&transform);
		}
	}

	GameObject::GameObject(Model *model, MaterialObject material, Animation *animation) : GameObject(model, material)
	{
		_animator = new Engine::Animator(animation);
		for (auto renderer : model->getMeshRenderers())
		{
			renderer->setAnimator(_animator);
		}
		for (auto renderer : model->getShadowRenderers())
		{
			renderer->setAnimator(_animator);
		}
	}

	void GameObject::setRigidBody(btRigidBody* rigidBody)
	{
		_rigidBody = rigidBody;
	}

	btRigidBody* GameObject::getRigidBody() const
	{
		return _rigidBody;
	}

	void GameObject::setRigidBodyCenterOffset(glm::vec3 offset)
	{
		_rigidBodyCenterOffset = offset;
	}

	void GameObject::syncTransformWithRigidBody()
	{
		if (_rigidBody == nullptr)
		{
			return;
		}

		btTransform rigidBodyTransform = _rigidBody->getWorldTransform();
		transform.position = glm::vec3(
			rigidBodyTransform.getOrigin().getX() + _rigidBodyCenterOffset.x,
			rigidBodyTransform.getOrigin().getY() + _rigidBodyCenterOffset.y,
			rigidBodyTransform.getOrigin().getZ() + _rigidBodyCenterOffset.z
		);
	}

	GameObject::GameObject(const GameObject *otherGameObject):
		BaseGameObject::BaseGameObject(otherGameObject)
	{
		Model *modelCopy = new Model(otherGameObject->getModel());
		_model = modelCopy;
		_material = MaterialObject();
		for (auto renderer : _model->getMeshRenderers())
		{
			renderer->setTransform(&transform);
		}
		for (auto renderer : _model->getShadowRenderers())
		{
			renderer->setTransform(&transform);
		}
		if (_animator) {
			for (auto renderer : _model->getMeshRenderers())
			{
				renderer->setAnimator(_animator);
			}
		}
		btRigidBody* otherRigidBody = otherGameObject->getRigidBody();
		if (otherRigidBody != nullptr)
		{
			setRigidBody(otherRigidBody);
		}
		setRigidBodyCenterOffset(otherGameObject->_rigidBodyCenterOffset);
	}

	GameObject *GameObject::clone() const
	{
		return new GameObject(this);
	}

	void GameObject::draw(Shader *shader) const
	{
		/*if (_model == nullptr)
		{
			return;
		}
		shader->use();
		_material.applyTextureOffset(shader);
		std::vector<Light *> lights = _scene->getLights();

		for (Light *light : lights)
		{
			light->apply(*shader);
		}

		Camera *camera = _scene->getActiveCamera();

		std::vector<MeshRenderer*> meshRenderers = _model->getMeshRenderers();
		for (size_t i = 0; i < meshRenderers.size(); ++i)
		{
			meshRenderers[i]->draw(camera->transform.position);
		}

		BaseGameObject::draw(shader);*/
	}

	void GameObject::calculateAabb()
	{
		auto mesh = _model->getMeshes()[0];
		_aabb = new Aabb(mesh->getVertexPositions());
	}

	Aabb* GameObject::getAabb() const
	{
		return _aabb;
	}

	GameObject::~GameObject()
	{

		BaseGameObject::~BaseGameObject();

		if (_aabb != nullptr)
		{
			delete _aabb;
		}
		
		delete _model;
	}

	Model *GameObject::getModel() const
	{
		return _model;
	}

	MaterialObject &GameObject::getMaterial()
	{
		return _material;
	}

	std::tuple<std::vector<std::shared_ptr<MeshRenderer>>, std::vector<std::shared_ptr<ShadowRenderer>>, std::vector<std::shared_ptr<TextRenderer>>> GameObject::getRenderers()
	{
		return { _model->getMeshRenderers(), _model->getShadowRenderers(), {} };
	}
}
