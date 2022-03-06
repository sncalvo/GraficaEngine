#pragma once

#include "../Core/Behaviour.h"

class Mover : public Engine::Behaviour
{
private:
	float _speed;
public:
	Mover(float speed);
	void update();
	Mover *clone() const override;
};
