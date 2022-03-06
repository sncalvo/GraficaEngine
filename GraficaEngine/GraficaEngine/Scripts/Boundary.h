#pragma once

#include "../Core/Behaviour.h"

class Boundary : public Engine::Behaviour
{
private:
	float _maxX;
	float _minX;
public:
	Boundary(float minX, float maxX);
	Boundary(Boundary *);
	Boundary *clone() const override;
	void update();
};