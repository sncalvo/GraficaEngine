#include "OrthographicCamera.h"
#include "Settings.h"

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

	glm::mat4 OrthographicCamera::getProjectionMatrixFov(const float fov) const
	{
	  return getProjectionMatrix();
	}

	Frustum OrthographicCamera::getFrustum() {
		Frustum frustum;
		float near, far;
		near = -100.f;
		far = 100.f;
		auto front = transform.getForward();
		auto up = transform.getUp();
		auto right = transform.getRight();

		frustum.nearFace = Plane(transform.position + near * front, front);
		frustum.farFace = Plane(transform.position + far * front, -front);
		frustum.rightFace = Plane(transform.position + 8.f * right, -right);
		frustum.leftFace = Plane(transform.position - 8.f * right, right);
		frustum.topFace = Plane(transform.position + 6.f * up, -up);
		frustum.bottomFace = Plane(transform.position - 6.f * up, up);

		return frustum;
	}
}
