#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

class Transform
{
private:
	glm::vec3 _up;
	glm::vec3 _forward;
	glm::mat4 _getTransformedModel() const;
public:
	Transform();
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 getUp();
	glm::vec3 getForward();
	void apply(Shader&) const;
};
