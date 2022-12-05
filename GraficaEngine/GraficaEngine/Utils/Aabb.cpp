#include "Aabb.h"

namespace Engine
{
    Aabb::Aabb(std::vector<glm::vec3> vertices) : position(glm::vec3(0.f))
    {
        auto constexpr infinity = std::numeric_limits<float>::infinity();
        _minVertex = glm::vec3(infinity);
        _maxVertex = glm::vec3(-infinity);

        for (const auto& vertex : vertices)
        {
            processVertex(vertex);
        }
    }

    void Aabb::processVertex(const glm::vec3& vertex)
    {
        _minVertex.x = glm::min(_minVertex.x, vertex.x);
        _minVertex.y = glm::min(_minVertex.y, vertex.y);
        _minVertex.z = glm::min(_minVertex.z, vertex.z);

        _maxVertex.x = glm::max(_maxVertex.x, vertex.x);
        _maxVertex.y = glm::max(_maxVertex.y, vertex.y);
        _maxVertex.z = glm::max(_maxVertex.z, vertex.z);
    }

    glm::vec3 Aabb::calculateMiddlePoint() const
    {
        return (_minVertex + _maxVertex) / 2.f;
    }

    glm::vec3 Aabb::getHalfExtents() const
    {
        auto difference = _maxVertex - _minVertex;
        return difference / 2.f;
    }

    bool isOnOrForwardPlane(const Plane& plane, glm::vec3& center, glm::vec3& extents) {
		// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
 		const float r = extents.x * std::abs(plane.normal.x) + extents.y * std::abs(plane.normal.y) +
			extents.z * std::abs(plane.normal.z);

		return -r <= plane.getSignedDistanceToPlane(center);
	}

	bool Aabb::isOnFrustum(const Frustum& camFrustum, const Transform& transform) const {
        glm::vec3 center = calculateMiddlePoint();
        glm::vec3 extents = getHalfExtents();
		//Get global scale thanks to our transform
        glm::vec4 tt = transform.getTransformedModel() * glm::vec4(center, 1.f);
		glm::vec3 globalCenter = glm::vec3(tt);

		// Scaled orientation
/* 		const glm::vec3 right = transform.getRight() * extents.x;
		const glm::vec3 up = transform.getUp() * extents.y;
		const glm::vec3 forward = transform.getForward() * extents.z;

		const float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
			std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
			std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

		const float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
			std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
			std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

		const float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
			std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
			std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

		extents = glm::vec3(newIi, newIj, newIk); */

		return (isOnOrForwardPlane(camFrustum.leftFace, globalCenter, extents) &&
                isOnOrForwardPlane(camFrustum.rightFace, globalCenter, extents) &&
                isOnOrForwardPlane(camFrustum.topFace, globalCenter, extents) &&
                isOnOrForwardPlane(camFrustum.bottomFace, globalCenter, extents) &&
                isOnOrForwardPlane(camFrustum.nearFace, globalCenter, extents) &&
                isOnOrForwardPlane(camFrustum.farFace, globalCenter, extents));
	};
}