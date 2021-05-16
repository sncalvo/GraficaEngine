#pragma once

#include "../Core/Behaviour.h"

class PlayerController : public Engine::Behaviour
{
private:
	float _speed;
	int _score;

public:
	PlayerController();
	void update();
	void die();
	void increaseScore(int amount);
};
