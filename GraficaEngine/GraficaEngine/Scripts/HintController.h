#pragma once

#include "../Core/Behaviour.h"
#include "../Utils/SmoothAcceleratedPosition.h"

class HintController : public Engine::Behaviour
{
private:
	float _hintDuration;
	float _direction;
	float _xDisplacement;
	float _position;
	SmoothAcceleratedPosition* _smoothPosition;
	bool _sliding;
public:
	HintController(float hintDuration, float xDisplacement);
	void start();
	void update();
	HintController(const HintController *);
	HintController *clone() const override;
};
