#pragma once

#include "../Core/Behaviour.h"

class TimeController : public Engine::Behaviour
{
private:
	float _time;
	int _minutes, _seconds;
public:
	TimeController();
	void update();
};
