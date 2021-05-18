#include "SmoothAcceleratedPosition.h"

#include <iostream>
#include <glm/glm.hpp>

SmoothAcceleratedPosition::SmoothAcceleratedPosition(float displacement) :
	_displacement(displacement),
	_totalTime(0.f)
{
}

float SmoothAcceleratedPosition::getPosition(float time)
{
	_totalTime += time;
	return _displacement + 7 - (((_displacement + 7) * glm::exp(-_totalTime)) - 7.f);
}
