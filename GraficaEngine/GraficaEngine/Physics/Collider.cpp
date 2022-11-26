#include "Collider.h"

#include "../Core/GameObject.h"

#include "../Utils/DebugLog.h"

namespace Engine
{
    Collider::Collider(glm::vec3 min, glm::vec3 max) : _min(min),
                                                       _max(max),
                                                       _gameObject(nullptr),
                                                       _mesh(_initializeMesh(min, max))
    {
    }

    Collider::Collider(Collider *otherCollider) : _min(otherCollider->_min),
                                                  _max(otherCollider->_max),
                                                  _gameObject(otherCollider->_gameObject),
                                                  _mesh(otherCollider->_mesh)
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

    glm::vec3 Collider::getMin() const
    {
        return _min;
    }

    glm::vec3 Collider::getMax() const
    {
        return _max;
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

        glm::vec3 minA = a->_min * a->_gameObject->transform.scale + a->_gameObject->transform.position;
        glm::vec3 maxA = a->_max * a->_gameObject->transform.scale + a->_gameObject->transform.position;
        glm::vec3 minB = b->_min * b->_gameObject->transform.scale + b->_gameObject->transform.position;
        glm::vec3 maxB = b->_max * b->_gameObject->transform.scale + b->_gameObject->transform.position;

        return (
            (minA.x <= maxB.x && maxA.x >= minB.x) &&
            (minA.y <= maxB.y && maxA.y >= minB.y) &&
            (minA.z <= maxB.z && maxA.z >= minB.z));
    }

    void Collider::draw(Shader *shaderPtr) const
    {
        Shader& shader = *shaderPtr;
        shader.use();

        auto goModel = _gameObject->transform.getTransformedModel();
        shader.setMatrix4f("model", glm::value_ptr(goModel));

        std::vector<Light *> lights = _gameObject->getScene()->getLights();

        for (Light *light : lights)
        {
            light->apply(shader);
        }

        Camera *camera = _gameObject->getScene()->getActiveCamera();
        camera->apply(shader);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // TODO: Collider will die in favor of bullet collisions. Won't fix
        // _mesh.draw(shader);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    Collider::~Collider()
    {
    }

    Mesh Collider::_initializeMesh(glm::vec3 min, glm::vec3 max)
    {
        std::vector<Vertex> vertices{
            Vertex{
                min,
                glm::vec3(0.f),
                glm::vec2(0.f)},
            Vertex{
                glm::vec3{min.x, max.y, min.z},
                glm::vec3(0.f),
                glm::vec2(0.f)},
            Vertex{
                glm::vec3{min.x, max.y, max.z},
                glm::vec3(0.f),
                glm::vec2(0.f)},
            Vertex{
                glm::vec3{min.x, min.y, max.z},
                glm::vec3(0.f),
                glm::vec2(0.f)},
            Vertex{
                glm::vec3{max.x, min.y, max.z},
                glm::vec3(0.f),
                glm::vec2(0.f)},
            Vertex{
                max,
                glm::vec3(0.f),
                glm::vec2(0.f)},
            Vertex{
                glm::vec3{max.x, max.y, min.z},
                glm::vec3(0.f),
                glm::vec2(0.f)},
            Vertex{
                glm::vec3{max.x, min.y, min.z},
                glm::vec3(0.f),
                glm::vec2(0.f)}};

        std::vector<unsigned int> indices{
            2, 1, 0,
            2, 0, 3,
            4, 2, 3,
            5, 2, 4,
            5, 4, 6,
            6, 4, 7,
            1, 6, 7,
            1, 7, 0,
            1, 2, 6,
            6, 2, 5,
            4, 3, 0,
            4, 0, 7};

        Material material{
            glm::vec3{0.4f, 1.f, 0.4f},
            glm::vec3{0.4f, 1.f, 0.4f},
            glm::vec3{0.4f, 1.f, 0.4f},
            1.f};

        return Mesh(vertices, indices, glm::vec3{0.f});
    }
}
