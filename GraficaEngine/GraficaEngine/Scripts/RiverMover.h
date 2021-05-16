#pragma once

#include "../Core/Behaviour.h"

class RiverMover : public Engine::Behaviour
{
private:
	float _movementSpeed = 0.5f;

public:
	RiverMover(float);
	RiverMover *clone() const;
	void update();
};
