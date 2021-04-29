#pragma once

#include "../Core/GameObject.h"
#include "../Core/Behaviour.h"
#include "../Core/Input.h"
#include "../Core/Time.h"

class PlayerController : public Engine::Behaviour
{
private:
	float _speed = 10.f;
public:
	PlayerController();
	void update();
};
