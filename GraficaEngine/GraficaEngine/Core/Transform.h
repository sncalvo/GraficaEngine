#pragma once

#include <glm/glm.hpp>

#include "../Renderer/Shader.h"

namespace Engine
{
	const glm::vec3 UP = glm::vec3(0.f, 1.f, 0.f);
	class Transform
	{
	private:
		glm::vec3 _up;
		glm::vec3 _forward;
		glm::vec3 _right;
		glm::mat4 _getTransformedModel() const;
		void _updateVectors();
		glm::vec3 _quaternionToEuler(glm::quat q);
		glm::vec3 _rotation;

	public:
		Transform();
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 getRotation() const;
		void setRotation(glm::vec3 rotation);
		void rotateX(float);
		void rotateY(float);
		void rotateZ(float);
		void setRotationX(float);
		void setRotationY(float);
		void setRotationZ(float);
		void lookAt(glm::vec3 direction);
		glm::vec3 getRight() const;
		glm::vec3 getUp() const;
		glm::vec3 getForward() const;
		void apply(Shader &, std::string uniform = "model") const;
	};
}
