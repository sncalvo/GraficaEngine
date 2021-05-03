#pragma once

#include "../Core/Behaviour.h"

class PlayerController : public Engine::Behaviour
{
private:
	float _speed = 10.f;

public:
	PlayerController();
	void update();
};
