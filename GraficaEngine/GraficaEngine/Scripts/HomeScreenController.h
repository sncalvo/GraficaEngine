#pragma once

#include "../Core/Behaviour.h"

class HomeScreenController : public Engine::Behaviour
{
private:
	bool _sliding;
public:
	HomeScreenController();
	void update();
	HomeScreenController* clone() const override;
};
