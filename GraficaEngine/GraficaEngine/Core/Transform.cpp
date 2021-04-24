#include <glm/gtc/type_ptr.hpp>

#include "Transform.h"

namespace Engine
{
	Transform::Transform() :
		position(glm::vec3{ 0.0, 0.0, 0.0 }),
		rotation(glm::vec3{ 0.0, 0.0, 0.0 }),
		scale(glm::vec3{ 1.0, 1.0, 1.0 }),
		_forward(glm::vec3{ 1.0, 0.0, 0.0 }),
		_up(glm::vec3{ 0.0, 1.0, 0.0 })
	{}

	glm::vec3 Transform::getUp()
	{
		return _up;
	}

	glm::vec3 Transform::getForward()
	{
		return _forward;
	}

	glm::mat4 Transform::_getTransformedModel() const
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, scale);

		// TODO: Optimize
		model = glm::rotate(model, rotation.x, glm::vec3(1.0, 0.0, 0.0));
		model = glm::rotate(model, rotation.y, glm::vec3(0.0, 1.0, 0.0));
		model = glm::rotate(model, rotation.z, glm::vec3(0.0, 0.0, 1.0));

		return model;
	}

	void Transform::apply(Shader& shader) const
	{
		glm::mat4 model = _getTransformedModel();
		shader.setMatrix4f("model", glm::value_ptr(model));
	}
}
