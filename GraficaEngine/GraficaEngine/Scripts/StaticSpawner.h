#pragma once

#include "../Core/Behaviour.h"
#include "../Core/GameObject.h"

class StaticSpawner : public Engine::Behaviour
{
protected:
	Engine::GameObject* _objectToSpawn;
	bool _canSpawn;
	bool _finished;

public:
	StaticSpawner()
	{};
	StaticSpawner(Engine::GameObject *objectToSpawn);
	virtual StaticSpawner* clone() const override;
	virtual void update();
	void startSpawning();
};
