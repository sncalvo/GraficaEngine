#include "PerspectiveCamera.h"

namespace Engine
{
	PerspectiveCamera::PerspectiveCamera(
		glm::vec3 position,
		glm::vec3 up,
		float yaw,
		float pitch
	) : Camera(position, up, yaw, pitch) {};

	glm::mat4 PerspectiveCamera::getProjectionMatrix() const
	{
		return glm::perspective(glm::radians(getZoom()), 800.0f / 600.0f, 0.1f, 100.0f); // TODO: Change parameters
	}
}
