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

    glm::vec3 Aabb::calculateMiddlePoint()
    {
        return (_minVertex + _maxVertex) / 2.f;
    }

    glm::vec3 Aabb::getHalfExtents() const
    {
        auto difference = _maxVertex - _minVertex;
        return difference / 2.f;
    }
}