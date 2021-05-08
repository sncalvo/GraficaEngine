#pragma once

#include <vector>

#include "../Core/Behaviour.h"
#include "../Core/GameObject.h"

class ObstacleSpawner : public Engine::Behaviour
{
private:
	std::vector<Engine::GameObject*> _obstacles;
	float _timeToNextSpawn;
	float _speed;
	bool _readyToSpawn() const;
public:
	ObstacleSpawner(std::vector<Engine::GameObject*> obstacles);
	void update();
};