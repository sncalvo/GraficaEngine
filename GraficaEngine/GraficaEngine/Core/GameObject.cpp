#include "GameObject.h"

namespace Engine
{
	GameObject::GameObject(Model *model, MaterialObject material) : _model(model), _material(material), _collider(nullptr)
	{
		for (auto renderer : model->getMeshRenderers())
		{
			renderer->setTransform(&transform);
		}
	}

	void GameObject::setCollider(Collider *collider)
	{
		collider->setGameObject(this);
		_collider = collider;
	}

	Collider *GameObject::getCollider() const
	{
		return _collider;
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
		Collider* otherCollider = otherGameObject->getCollider();
		if (otherCollider != nullptr)
		{
			setCollider(new Collider(otherGameObject->getCollider()));
		}
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

	GameObject::~GameObject()
	{

		BaseGameObject::~BaseGameObject();

		if (_collider != nullptr)
		{
			_scene->removeCollider(_collider);
			delete _collider;
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
