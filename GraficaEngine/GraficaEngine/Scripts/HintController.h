#pragma once

#include "../Core/Behaviour.h"

class HintController : public Engine::Behaviour
{
private:
	float _hintDuration;
	float _direction;
	float _xDisplacement;
	float _position;
	bool _sliding;
public:
	HintController(float hintDuration, float xDisplacement);
	void update();
	HintController(const HintController *);
	HintController *clone() const override;
};
