#pragma once

#include <glm/glm.hpp>

#include "../Core/Behaviour.h"

class JumpController : public Engine::Behaviour
{
private:
	float _gravity = -9.8f;
	float _mass = 1.f;
	float _jumpStrength = 1.f;
	bool _grounded = false;
	glm::vec3 _velocity = glm::vec3(0.f);

	bool _shouldJump();
	void _updatePosition();
	void _updateVelocity();

public:
	void update();
	JumpController();
};
