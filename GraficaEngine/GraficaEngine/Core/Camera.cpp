#include "Camera.h"

#include <glm/gtc/type_ptr.hpp>

namespace Engine
{
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch):
        _front(glm::vec3(0.0f, 0.0f, -1.0f)),
        _movementSpeed(SPEED),
        _mouseSensitivity(SENSITIVITY),
        _zoom(ZOOM),
        _position(position),
        _worldUp(up),
        _yaw(yaw),
        _pitch(pitch)
    {
        _updateCameraVectors();
    }

    glm::mat4 Camera::getViewMatrix() const
    {
        return glm::lookAt(_position, _position + _front, _up);
    }

    void Camera::apply(Shader& shader) const
    {
        shader.setVec3f("viewPos", glm::value_ptr(_position));
    }

    void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = _movementSpeed * deltaTime;
        if (direction == Camera_Movement::FORWARD)
            _position += _front * velocity;
        if (direction == Camera_Movement::BACKWARD)
            _position -= _front * velocity;
        if (direction == Camera_Movement::LEFT)
            _position -= _right * velocity;
        if (direction == Camera_Movement::RIGHT)
            _position += _right * velocity;
    }

    void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
    {
        xoffset *= _mouseSensitivity;
        yoffset *= _mouseSensitivity;

        _yaw += xoffset;
        _pitch += yoffset;

        if (constrainPitch)
        {
            if (_pitch > 89.0f)
                _pitch = 89.0f;
            if (_pitch < -89.0f)
                _pitch = -89.0f;
        }

        _updateCameraVectors();
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

    void Camera::_updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        front.y = sin(glm::radians(_pitch));
        front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        _front = glm::normalize(front);

        _right = glm::normalize(glm::cross(_front, _worldUp));
        _up = glm::normalize(glm::cross(_right, _front));
    }
}