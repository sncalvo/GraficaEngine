#include "Aabb.h"

namespace Engine
{
    Aabb::Aabb(std::vector<glm::vec3> vertices)
    {
        auto constexpr infinity = std::numeric_limits<float>::infinity();
        minVertex = glm::vec3(infinity);
        maxVertex = glm::vec3(-infinity);

        for (const auto& vertex : vertices)
        {
            processVertex(vertex);
        }
    }

    void Aabb::processVertex(const glm::vec3& vertex)
    {
        minVertex.x = glm::min(minVertex.x, vertex.x);
        minVertex.y = glm::min(minVertex.y, vertex.y);
        minVertex.z = glm::min(minVertex.z, vertex.z);

        maxVertex.x = glm::max(maxVertex.x, vertex.x);
        maxVertex.y = glm::max(maxVertex.y, vertex.y);
        maxVertex.z = glm::max(maxVertex.z, vertex.z);
    }

    glm::vec3 Aabb::calculateMiddlePoint()
    {
        return (minVertex + maxVertex) / 2.f;
    }

    glm::vec3 Aabb::getLargestContainingBox()
    {
        auto differenceVector = maxVertex - minVertex;
        
        if (differenceVector.x > glm::max(differenceVector.y, differenceVector.z))
        {
            return glm::vec3(differenceVector.x / 2.f);
        }

        if (differenceVector.y > glm::max(differenceVector.x, differenceVector.z))
        {
            return glm::vec3(differenceVector.y / 2.f);
        }

        if (differenceVector.z > glm::max(differenceVector.x, differenceVector.y))
        {
            return glm::vec3(differenceVector.z / 2.f);
        }
    }
}