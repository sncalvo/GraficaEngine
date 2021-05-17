#include "PerspectiveCamera.h"

namespace Engine
{
	PerspectiveCamera::PerspectiveCamera(
		glm::vec3 position,
		glm::vec3 up,
		float yaw,
		float pitch
	) : Camera(position, up, yaw, pitch) {};

	PerspectiveCamera::PerspectiveCamera(const PerspectiveCamera *otherCamera)
		: Camera(otherCamera)
	{
	}

	PerspectiveCamera *PerspectiveCamera::clone() const
	{
		return new PerspectiveCamera(this);
	}

	glm::mat4 PerspectiveCamera::getProjectionMatrix() const
	{
		return glm::perspective(glm::radians(getZoom()), 800.0f / 600.0f, 0.1f, 100.0f); // TODO: Change parameters
	}
}
