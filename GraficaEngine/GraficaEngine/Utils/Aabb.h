#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace Engine
{
    class Aabb
    {
    private:
        glm::vec3 minVertex, maxVertex;
        void processVertex(const glm::vec3& vertex);
    public:
        Aabb(std::vector<glm::vec3> vertices);
        glm::vec3 calculateMiddlePoint();
        glm::vec3 getLargestContainingBox();
    };
}