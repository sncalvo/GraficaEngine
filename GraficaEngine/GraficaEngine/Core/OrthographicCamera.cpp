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
		return glm::ortho(-8.0f, 8.0f, -6.0f, 6.0f, -100.0f, 100.0f); // TODO: Change parameters
	}
}
