#pragma once

#include "../Core/Behaviour.h"

class GameKeysHomeController : public Engine::Behaviour
{
private:
	float _time;
	bool _switchColor;
	bool _sliding;
public:
	GameKeysHomeController();
	void update();
	GameKeysHomeController* clone() const override;
};
