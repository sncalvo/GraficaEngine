#pragma once

#include "../Core/Behaviour.h"

class CoinController : public Engine::Behaviour
{
private:
	float _dyingCountdown;
	bool _dying;
public:
	CoinController();
	void update();
	CoinController* clone() const override;
};
