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
		float near, far;

		std::tie(near, far) = Settings::getInstance().getCameraNearAndFarPlane();

		return getProjectionMatrixFor(near, far);
	}

	glm::mat4 PerspectiveCamera::getProjectionMatrixFor(const float near, const float far) const
	{
		unsigned int width, height;
		std::tie(width, height) = Settings::getInstance().getWindowSize();
		return glm::perspective(glm::radians(getZoom()), (float)width / (float)height, near, far);
	}

	glm::mat4 PerspectiveCamera::getProjectionMatrixFov(const float fov) const
	{
		unsigned int width, height;
		std::tie(width, height) = Settings::getInstance().getWindowSize();
		return glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.f); // TODO: Change parameters
	}
}
