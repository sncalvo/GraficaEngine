#pragma once

#include <vector>

#include <gl/glew.h>
#include <SDL/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "BaseGameObject.h"
#include "../Renderer/Shader.h"

namespace Engine
{
    enum class Camera_Movement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float SPEED = 10.f;
    const float SENSITIVITY = 50.f;
    const float ZOOM = 45.0f;

    class Camera : public BaseGameObject
    {
    public:
        Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
        Camera(const Camera *);
        Camera *clone() const;
        glm::mat4 getViewMatrix() const;
        glm::mat4 getBoxMatrix();
        virtual glm::mat4 getProjectionMatrix() const;
        virtual glm::mat4 getProjectionMatrixFor(const float near, const float far) const;
        virtual glm::mat4 getProjectionMatrixFov(const float fov) const;
        std::vector<glm::vec4> getFrustumCornersWorldSpace(const float near, const float far) const;
        void apply(Shader &shader) const;
        void processKeyboard(Camera_Movement direction, float deltaTime);
        void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
        void processMouseScroll(float yoffset);
        float getZoom() const;

    private:
        glm::vec3 _front;
        glm::vec3 _up;
        glm::vec3 _right;
        glm::vec3 _worldUp;

        float _yaw;
        float _pitch;
        float _movementSpeed;
        float _mouseSensitivity;
        float _zoom;
    };
}
