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
	float _sign;
	bool _canSpawn;
public:
	ObstacleSpawner(std::vector<Engine::GameObject*> obstacles);
	ObstacleSpawner(const ObstacleSpawner *);
	ObstacleSpawner *clone() const override;
	void update();
	void startSpawning();
};