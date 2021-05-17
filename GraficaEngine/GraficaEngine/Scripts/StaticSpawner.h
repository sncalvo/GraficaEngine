#pragma once

#include "../Core/Behaviour.h"
#include "../Core/GameObject.h"

class StaticSpawner : public Engine::Behaviour
{
private:
	Engine::GameObject* _objectToSpawn;
	bool _canSpawn;
	bool _finished;
public:
	StaticSpawner(Engine::GameObject *objectToSpawn);
	StaticSpawner* clone() const override;
	void update();
	void startSpawning();
};
