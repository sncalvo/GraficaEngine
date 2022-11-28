#pragma once

#include <glm/glm.hpp>

#include "../Core/Behaviour.h"

class JumpController : public Engine::Behaviour
{
private:
	float _jumpStrength = 1.f;
	bool _grounded = false;
	
	bool _shouldJump();
	void _updateGrounded();

public:
	void update();
	JumpController();
	JumpController(const JumpController *);
	JumpController *clone() const override;
};
