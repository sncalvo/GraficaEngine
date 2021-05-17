#pragma once

#include "glm/glm.hpp"

#include "../Core/Behaviour.h"

class ThirdPersonCameraController : public Engine::Behaviour
{
private:
     float _distance;
     float _sensitivityX = 2.f;
     float _sensitivityY = 2.f;
     glm::vec3 _offset;
     glm::vec2 _xyMovement;

public:
    ThirdPersonCameraController(float distance, glm::vec3 offset);
    void update();
    ThirdPersonCameraController *clone() const override;
};
