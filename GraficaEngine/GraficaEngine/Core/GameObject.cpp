#include "GameObject.h"

namespace Engine
{
	GameObject::GameObject(Model *model, MaterialObject material) : _model(model), _material(material), _collider(nullptr)
	{
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

	GameObject::GameObject(GameObject *otherGameObject):
		BaseGameObject::BaseGameObject(otherGameObject)
	{
		Model *modelCopy = new Model(otherGameObject->getModel());
		_model = modelCopy;
		_material = MaterialObject(otherGameObject->getMaterial().getShader());
		Collider* otherCollider = otherGameObject->getCollider();
		if (otherCollider != nullptr)
		{
			setCollider(new Collider(otherGameObject->getCollider()));
		}
	}

	void GameObject::draw() const
	{
		if (_model == nullptr)
		{
			return;
		}
		Shader *shader = _material.getShader();
		shader->use();
		transform.apply(*shader);
		std::vector<Light *> lights = _scene->getLights();

		for (Light *light : lights)
		{
			light->apply(*shader);
		}

		Camera *camera = _scene->getActiveCamera();
		camera->apply(*shader);

		_model->draw(*shader);

		BaseGameObject::draw();
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

	MaterialObject GameObject::getMaterial() const
	{
		return _material;
	}
}
