#pragma once

#include "Camera.h"

namespace Engine
{
	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
			float yaw = YAW,
			float pitch = PITCH
		);
		virtual glm::mat4 getProjectionMatrix() const;
	};
}
