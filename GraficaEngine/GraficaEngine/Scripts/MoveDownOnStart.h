#pragma once

#include "../Core/Behaviour.h"
#include "../Utils/SmoothAcceleratedPosition.h"

class MoveDownOnStart : public Engine::Behaviour
{
private:
	float _positionMoved;
	float _finishAnimation;
	float _height;
	float _position;
	SmoothAcceleratedPosition* _smoothPosition;

	bool _startAnimation;

public:
	MoveDownOnStart(float yDisplacement);
	void update();
	void start();
	MoveDownOnStart* clone() const override;
	~MoveDownOnStart();
};
