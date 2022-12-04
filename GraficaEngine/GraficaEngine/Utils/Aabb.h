#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Frustum.h"
#include "../Core/Transform.h"

namespace Engine {
    class Aabb {
    private:
        glm::vec3 _minVertex, _maxVertex;
        void processVertex(const glm::vec3& vertex);
    public:
        glm::vec3 position;
        Aabb(std::vector<glm::vec3> vertices);
        glm::vec3 calculateMiddlePoint() const;
        glm::vec3 getHalfExtents() const;
        bool isOnFrustum(const Frustum& camFrustum, const Transform& transform) const;
    };
}