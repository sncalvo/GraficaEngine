#include "GameObject.h"

namespace Engine
{
	GameObject::GameObject(Model* model, MaterialObject material) : _model(model), _material(material)
	{}

	void GameObject::draw() const
	{
		if (_model == nullptr)
		{
			return;
		}
		Shader* shader = _material.getShader();
		shader->use();
		transform.apply(*shader);
		std::vector<Light*> lights = _scene->getLights();

		for (Light* light : lights)
		{
			light->apply(*shader);
		}

		Camera* camera = _scene->getActiveCamera();
		camera->apply(*shader);

		_model->draw(*shader);
	}

	GameObject::~GameObject()
	{
		delete _model;

		_scene->removeGameObject(this);
		_scene = nullptr;

		BaseGameObject::~BaseGameObject();
	}

	bool GameObject::isDrawable() const
	{
		return true;
	}
}
