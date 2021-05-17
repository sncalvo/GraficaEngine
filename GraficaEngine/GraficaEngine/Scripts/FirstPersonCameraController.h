#pragma once

#include "glm/glm.hpp"

#include "../Core/Behaviour.h"

class FirstPersonCameraController : public Engine::Behaviour
{
private:
	glm::vec3 _relativePosition;

public:
	FirstPersonCameraController(glm::vec3 relativePos);
	FirstPersonCameraController(const FirstPersonCameraController *);
	void update();
	FirstPersonCameraController *clone() const override;
};
