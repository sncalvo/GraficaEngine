#pragma once

#include "../Core/Behaviour.h"

class Hazard : public Engine::Behaviour
{
private:
	void _checkCollision();

public:
	void update();
	Hazard *clone() const;
};
