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

	Frustum PerspectiveCamera::getFrustum() {
		Frustum frustum;
		float near, far;
		std::tie(near, far) = Settings::getInstance().getCameraNearAndFarPlane();
		unsigned int width, height;
		std::tie(width, height) = Settings::getInstance().getWindowSize();

		auto front = transform.getForward();
		auto up = transform.getUp();
		auto right = transform.getRight();

		auto aspect = (float)width / (float)height;
		auto fov = glm::radians(getZoom());
		const float halfVSide = far * tanf(fov * .5f);
		const float halfHSide = halfVSide * aspect;
		const glm::vec3 frontMultFar = far * front;

		frustum.nearFace = Plane(transform.position + near * front, front);
		frustum.farFace = Plane(transform.position + frontMultFar, -front);
		frustum.rightFace = Plane(transform.position, glm::cross(up,frontMultFar + right * halfHSide));
		frustum.leftFace = Plane(transform.position, glm::cross(frontMultFar - right * halfHSide, up));
		frustum.topFace = Plane(transform.position, glm::cross(right, frontMultFar - up * halfVSide));
		frustum.bottomFace = Plane(transform.position, glm::cross(frontMultFar + up * halfVSide, right));

		return frustum;
	}
}
