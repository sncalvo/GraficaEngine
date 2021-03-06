#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "Transform.h"

#include "../Utils/DebugLog.h"

namespace Engine
{
	Transform::Transform() : position(glm::vec3(0.f)),
							 _rotation(glm::vec3(0.f)),
							 scale(glm::vec3(1.f))
	{
		_updateVectors();
	}

	glm::vec3 Transform::getRight() const
	{
		return _right;
	}

	glm::vec3 Transform::getUp() const
	{
		return _up;
	}

	glm::vec3 Transform::getForward() const
	{
		return _forward;
	}

	glm::vec3 Transform::getRotation() const
	{
		return _rotation;
	}

	void Transform::setRotation(glm::vec3 rotation)
	{
		_rotation = rotation;
		_updateVectors();
	}

	void Transform::rotateX(float x)
	{
		_rotation.x += x;
		_updateVectors();
	}

	void Transform::rotateY(float y)
	{
		_rotation.y += y;
		_updateVectors();
	}
	void Transform::rotateZ(float z)
	{
		_rotation.z += z;
		_updateVectors();
	}

	void Transform::setRotationX(float x)
	{
		_rotation.x = x;
		_updateVectors();
	}

	void Transform::setRotationY(float y)
	{
		_rotation.y = y;
		_updateVectors();
	}

	void Transform::setRotationZ(float z)
	{
		_rotation.z = z;
		_updateVectors();
	}

	void Transform::lookAt(glm::vec3 direction)
	{
		_forward = glm::normalize(direction);
		auto quatLook = glm::quatLookAt(_forward, UP);

		_rotation = _quaternionToEuler(quatLook);
		if (_forward.z > 0)
		{
			_rotation.y = -_rotation.y;
		}

		_right = glm::normalize(glm::cross(_forward, UP));
		_up = glm::normalize(glm::cross(_right, _forward));
	}

	void Transform::_updateVectors()
	{
		glm::vec3 forward;
		forward.x = cos(glm::radians(_rotation.y)) * cos(glm::radians(_rotation.x));
		forward.y = sin(glm::radians(_rotation.x));
		forward.z = sin(glm::radians(_rotation.y)) * cos(glm::radians(_rotation.x));
		_forward = glm::normalize(forward);

		_right = glm::normalize(glm::cross(_forward, UP));
		_up = glm::normalize(glm::cross(_right, _forward));
	}

	glm::mat4 Transform::_getTransformedModel() const
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, scale);

		model *= glm::eulerAngleYXZ(
			glm::radians(_rotation.y),
			glm::radians(_rotation.x),
			glm::radians(_rotation.z));

		return model;
	}

	void Transform::apply(Shader &shader, std::string uniform) const
	{
		glm::mat4 model = _getTransformedModel();
		shader.setMatrix4f(uniform, glm::value_ptr(model));
	}

	glm::vec3 Transform::_quaternionToEuler(glm::quat q)
	{
		glm::vec3 euler = glm::eulerAngles(q);

		return glm::vec3(glm::degrees(euler.x), glm::degrees(euler.y), glm::degrees(euler.z));
	}
}
