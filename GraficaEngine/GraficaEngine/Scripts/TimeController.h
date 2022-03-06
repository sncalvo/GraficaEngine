#pragma once

#include "../Core/Behaviour.h"

class TimeController : public Engine::Behaviour
{
private:
	float _time;
	int _minutes, _seconds;
	bool _shouldUpdate;
public:
	TimeController();
	void update();
	TimeController *clone() const override;
};
