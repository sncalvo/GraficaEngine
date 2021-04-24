#include "GameObject.h"

namespace Engine
{
	GameObject::GameObject(Model model, MaterialObject material) : _model(model), _material(material)
	{}

	void GameObject::draw() const
	{
		Shader* shader = _material.getShader();
		shader->use();
		transform.apply(*shader);
		std::vector<Light*> lights = _scene->getLights();

		for (Light* light : lights)
		{
			light->apply(*shader);
		}

		Camera* camera = _scene->getCamera();
		camera->apply(*shader);

		_model.draw(*shader);
	}

	void GameObject::update()
	{
		for (Behaviour* behaviour : _behaviours)
		{
			behaviour->update();
		}
	}

	void GameObject::setScene(Scene* scene)
	{
		_scene = scene;
	}

	Scene* GameObject::getScene()
	{
		return _scene;
	}

	void GameObject::addBehaviour(Behaviour* behaviour)
	{
		behaviour->setGameObject(this);
		_behaviours.push_back(behaviour);
	}

	GameObject::~GameObject()
	{
		_scene->removeGameObject(this);
		_scene = nullptr;

		for (Behaviour* behaviour : _behaviours)
		{
			delete behaviour;
		}
	}
}