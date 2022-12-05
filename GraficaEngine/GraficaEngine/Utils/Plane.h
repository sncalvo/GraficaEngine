#pragma once
#include <glm/vec3.hpp>

struct Plane {
    // unit vector
    glm::vec3 normal = { 0.f, 1.f, 0.f };

    // distance from origin to the nearest point in the plane
    float distance = 0.f;             
    Plane() = default;

	Plane(const glm::vec3& p1, const glm::vec3& norm)
		: normal(glm::normalize(norm)),
		distance(glm::dot(normal, p1))
	{}

	float getSignedDistanceToPlane(const glm::vec3& point) const
	{
		return (glm::dot(normal, point) - distance);
	}
};