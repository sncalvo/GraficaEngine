#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "../Renderer/Shader.h"
#include "../Renderer/Mesh.h"

namespace Engine
{
    class GameObject;

    class Collider
    {
    private:
        glm::vec3 _min, _max;
        std::vector<Collider *> _collidingWith;
        GameObject *_gameObject;
        Mesh _mesh;

        Mesh _initializeMesh(glm::vec3 min, glm::vec3 max);

    public:
        Collider(glm::vec3 min, glm::vec3 max);
        Collider(Collider *);
        void resetCollisions();
        void collide(Collider *other);
        void setGameObject(GameObject *gameObject);
        glm::vec3 getMin() const;
        glm::vec3 getMax() const;
        std::vector<Collider *> getCollisions() const;
        GameObject *getGameObject() const;
        void draw() const;
        static bool intersect(Collider *, Collider *);
        ~Collider();
    };
}
