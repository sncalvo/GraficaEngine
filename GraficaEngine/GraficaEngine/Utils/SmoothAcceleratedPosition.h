#pragma once

class SmoothAcceleratedPosition
{
private:
	float _displacement;
	float _totalTime;
public:
	SmoothAcceleratedPosition(float displacement);
	float getPosition(float time);
};
