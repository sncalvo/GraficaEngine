#include "PerspectiveCamera.h"

#include "Settings.h"

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
		return getProjectionMatrixFor(0.1f, 500.f); // TODO: Change parameters
	}

	glm::mat4 PerspectiveCamera::getProjectionMatrixFor(const float near, const float far) const
	{
		unsigned int width, height;
		std::tie(width, height) = Settings::getInstance().getWindowSize();
		return glm::perspective(glm::radians(getZoom()), (float)width / (float)height, near, far); // TODO: Change parameters
	}
}
