#include "Camera.h"
#include "../Utils/DebugLog.h"

#include <glm/gtc/type_ptr.hpp>

namespace Engine
{
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : _front(glm::vec3(0.0f, 0.0f, -1.0f)),
                                                                               _movementSpeed(SPEED),
                                                                               _mouseSensitivity(SENSITIVITY),
                                                                               _zoom(ZOOM),
                                                                               _worldUp(up),
                                                                               _yaw(yaw),
                                                                               _pitch(pitch)
    {
        transform.setRotationX(pitch);
        transform.setRotationY(yaw);
        transform.position = position;
    }

    Camera::Camera(const Camera *otherCamera) :
        _front(otherCamera->_front),
        _up(otherCamera->_up),
        _right(otherCamera->_right),
        _worldUp(otherCamera->_worldUp),
        _yaw(otherCamera->_yaw),
        _pitch(otherCamera->_pitch),
        _movementSpeed(otherCamera->_movementSpeed),
        _mouseSensitivity(otherCamera->_mouseSensitivity),
        _zoom(otherCamera->_zoom),
        BaseGameObject(otherCamera)
    {
    }

    Camera *Camera::clone() const
    {
        return new Camera(this);
    }

    glm::mat4 Camera::getViewMatrix() const
    {
        return glm::lookAt(
            transform.position,
            transform.position + transform.getForward(),
            transform.getUp());
    }

    std::vector<glm::vec4> Camera::getFrustumCornersWorldSpace() const
    {
        auto view = getViewMatrix();
        auto projection = getProjectionMatrix();

        const auto inv = glm::inverse(projection * view);

        std::vector<glm::vec4> frustumCorners;
        for (unsigned int x = 0; x < 2; ++x)
        {
            for (unsigned int y = 0; y < 2; ++y)
            {
                for (unsigned int z = 0; z < 2; ++z)
                {
                    const glm::vec4 pt =
                        inv * glm::vec4(
                            2.0f * x - 1.0f,
                            2.0f * y - 1.0f,
                            2.0f * z - 1.0f,
                            1.0f);
                    frustumCorners.push_back(pt / pt.w);
                }
            }
        }

        return frustumCorners;
    }

    void Camera::apply(Shader &shader) const
    {
        shader.setVec3f("viewPos", glm::value_ptr(transform.position));
    }

    void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = _movementSpeed * deltaTime;
        if (direction == Camera_Movement::FORWARD)
            transform.position += transform.getForward() * velocity;
        if (direction == Camera_Movement::BACKWARD)
            transform.position -= transform.getForward() * velocity;
        if (direction == Camera_Movement::LEFT)
            transform.position -= transform.getRight() * velocity;
        if (direction == Camera_Movement::RIGHT)
            transform.position += transform.getRight() * velocity;
    }

    void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
    {
        xoffset *= _mouseSensitivity;
        yoffset *= _mouseSensitivity;

        transform.rotateY(xoffset);
        transform.rotateX(yoffset);

        if (constrainPitch)
        {
            if (transform.getRotation().x > 89.0f)
                transform.setRotationX(89.0f);
            if (transform.getRotation().x < -89.0f)
                transform.setRotationX(-89.0f);
        }
    }

    void Camera::processMouseScroll(float yoffset)
    {
        _zoom -= (float)yoffset;
        if (_zoom < 1.0f)
            _zoom = 1.0f;
        if (_zoom > 45.0f)
            _zoom = 45.0f;
    }

    float Camera::getZoom() const
    {
        return _zoom;
    }

    glm::mat4 Camera::getProjectionMatrix() const
    {
        return glm::mat4(0.f);
    }
}
