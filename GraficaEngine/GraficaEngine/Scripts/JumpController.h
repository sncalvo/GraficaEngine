#pragma once

#include <glm/glm.hpp>

#include "../Core/Behaviour.h"

class JumpController : public Engine::Behaviour
{
private:
	float _gravity = -9.8f;
	float _mass = 1.f;
	float _jumpStrength = 1.f;
	glm::vec3 _velocity = glm::vec3(0.f);

	bool _shouldJump();
	void _updatePosition();
	void _updateVelocity();
	void _updateGrounded();

	bool _grounded = false;

public:
	void update();
	JumpController();
	JumpController(const JumpController *);
	JumpController *clone() const override;
};
