#pragma once

#include "../Core/Behaviour.h"

class PlayerController : public Engine::Behaviour
{
private:
	float _speed;

public:
	PlayerController();
	void update();
	void die();
};
