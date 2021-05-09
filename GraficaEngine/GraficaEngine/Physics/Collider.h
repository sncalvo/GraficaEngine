#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace Engine
{
    class GameObject;

    class Collider
    {
    private:
        glm::vec3 _min, _max;
        std::vector<Collider *> _collidingWith;
        GameObject *_gameObject;

    public:
        Collider(glm::vec3 min, glm::vec3 max);
        void resetCollisions();
        void collide(Collider *other);
        void setGameObject(GameObject *gameObject);
        std::vector<Collider* > getCollisions() const;
        GameObject *getGameObject() const;
        static bool intersect(Collider *, Collider *);
        ~Collider();
    };
}
