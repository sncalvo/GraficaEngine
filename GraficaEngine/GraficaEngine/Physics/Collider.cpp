#include "Collider.h"

#include "../Core/GameObject.h"

#include "../Utils/DebugLog.h"

namespace Engine
{
    Collider::Collider(glm::vec3 min, glm::vec3 max) : _min(min), _max(max), _gameObject(nullptr)
    {
    }

    void Collider::resetCollisions()
    {
        _collidingWith.clear();
    }

    void Collider::collide(Collider *other)
    {
        _collidingWith.push_back(other);
    }

    void Collider::setGameObject(GameObject *gameObject)
    {
        _gameObject = gameObject;
    }

    GameObject *Collider::getGameObject() const
    {
        return _gameObject;
    }

    std::vector<Collider *> Collider::getCollisions() const
    {
        return _collidingWith;
    }

    bool Collider::intersect(Collider *a, Collider *b)
    {
        if (a == b)
        {
            return false;
        }

        glm::vec3 minA = a->_min + a->_gameObject->transform.position;
        glm::vec3 maxA = a->_max + a->_gameObject->transform.position;
        glm::vec3 minB = b->_min + b->_gameObject->transform.position;
        glm::vec3 maxB = b->_max + b->_gameObject->transform.position;

        return (
            (minA.x <= maxB.x && maxA.x >= minB.x) &&
            (minA.y <= maxB.y && maxA.y >= minB.y) &&
            (minA.z <= maxB.z && maxA.z >= minB.z));
    }

    Collider::~Collider()
    {
    }
}
