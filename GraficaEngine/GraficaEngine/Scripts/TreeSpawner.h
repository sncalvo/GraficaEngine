#pragma once

#include "../Core/Behaviour.h"
#include "../Core/GameObject.h"

#include "StaticSpawner.h"

class TreeSpawner : public StaticSpawner
{
public:
	TreeSpawner(Engine::GameObject* objectToSpawn);
	TreeSpawner* clone() const override;
	void update();
};
