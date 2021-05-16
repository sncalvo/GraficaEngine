#pragma once

#include "../Core/Behaviour.h"
#include "../Core/GameObject.h"

class StaticSpawner : public Engine::Behaviour
{
private:
	Engine::GameObject* _objectToSpawn;
public:
	StaticSpawner(Engine::GameObject *objectToSpawn);
	StaticSpawner* clone() const;
	void start();
};
