#pragma once

#include "../Core/Behaviour.h"

class PlayerController : public Engine::Behaviour
{
private:
	float _speed;
	int _score;
	bool _canMove;

public:
	PlayerController();
	void update();
	void die();
	void increaseScore(int amount);
	PlayerController *clone() const override;
	void setCanMove(bool canMove);
};
