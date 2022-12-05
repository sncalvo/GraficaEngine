#pragma once

#include "../Core/Behaviour.h"
#include "../Core/GameObject.h"

class ShootBalls : public Engine::Behaviour
{
public:
	ShootBalls(Engine::GameObject* ballPrefab);
	ShootBalls* clone() const;
	void update();
private:
	Engine::GameObject* _ballPrefab;
	float _speed;
};