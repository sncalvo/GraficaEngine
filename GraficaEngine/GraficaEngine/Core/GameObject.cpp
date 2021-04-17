#include "GameObject.h"

namespace Engine
{
	GameObject::GameObject(Model model, MaterialObject material) : _model(model), _material(material) {}

	void GameObject::draw() const
	{
		Shader* shader = _material.getShader();
		shader->use();
		transform.apply(*shader);
		_model.draw(*shader);
	}
}