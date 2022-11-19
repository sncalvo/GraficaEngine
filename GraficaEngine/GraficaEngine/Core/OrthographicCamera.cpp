#include "OrthographicCamera.h"

namespace Engine
{
	OrthographicCamera::OrthographicCamera(
		glm::vec3 position,
		glm::vec3 up,
		float yaw,
		float pitch
	) : Camera(position, up, yaw, pitch) {};

	OrthographicCamera::OrthographicCamera(const OrthographicCamera *otherCamera) :
		Camera(otherCamera)
	{
	}

	OrthographicCamera *OrthographicCamera::clone() const
	{
		return new OrthographicCamera(this);
	}

	glm::mat4 OrthographicCamera::getProjectionMatrix() const
	{
		return getProjectionMatrixFor(-100.f, 100.f); // TODO: Change parameters
	}

	glm::mat4 OrthographicCamera::getProjectionMatrixFor(const float near, const float far) const
	{
		return glm::ortho(-8.0f, 8.0f, -6.0f, 6.0f, near, far); // TODO: Change parameters
	}
}
