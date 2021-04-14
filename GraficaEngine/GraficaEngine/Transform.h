#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
private:
	glm::vec3 _up;
	glm::vec3 _forward;

public:
	Transform();
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 getUp();
	glm::vec3 getForward();
	glm::mat4 getTransformedModel();
};
