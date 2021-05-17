#pragma once

#include "../Core/Behaviour.h"

class Riser : public Engine::Behaviour {
private:
	float _stopHeight;
	float _speed;
	bool _finished;
public:
	Riser(float stopHeight = 0.f, float speed = 5.f);
	void update();
	Riser *clone() const override;
};