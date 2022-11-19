#pragma once

#include "Camera.h"

namespace Engine
{
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
			float yaw = YAW,
			float pitch = PITCH
		);
		PerspectiveCamera(const PerspectiveCamera *);
		PerspectiveCamera *clone() const;
		virtual glm::mat4 getProjectionMatrix() const;
		virtual glm::mat4 getProjectionMatrixFor(const float near, const float far) const;
	};
}
