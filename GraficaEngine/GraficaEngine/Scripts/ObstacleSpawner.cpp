#include "ObstacleSpawner.h"

#include <stdlib.h>
#include <time.h>

#include "../Core/Time.h"
#include "Mover.h"
#include "Boundary.h"
#include "Hazard.h"

ObstacleSpawner::ObstacleSpawner(std::vector<Engine::GameObject *> obstacles) : _obstacles(obstacles)
{
	srand(time(nullptr));
	_timeToNextSpawn = Engine::Time::getTime() + rand() % 5;
	_speed = rand() % 5 + 5.f;
}

ObstacleSpawner::ObstacleSpawner(ObstacleSpawner *otherObstacleSpawner): _obstacles(otherObstacleSpawner->_obstacles)
{
	srand(time(nullptr));
	_timeToNextSpawn = Engine::Time::getTime() + rand() % 5;
	_speed = rand() % 5 + 5.f;
}

ObstacleSpawner *ObstacleSpawner::clone() const
{
	return new ObstacleSpawner(*this);
}

bool ObstacleSpawner::_readyToSpawn() const
{
	return Engine::Time::getTime() >= _timeToNextSpawn;
}

void ObstacleSpawner::update()
{
	if (_readyToSpawn())
	{
		float spawnRate = rand() % 5 + 2;
		_timeToNextSpawn = Engine::Time::getTime() + spawnRate;

		int randomIndex = rand() % _obstacles.size();

		Engine::GameObject *randomObstacle = _obstacles[randomIndex];
		Engine::GameObject *obstacleToSpawn = new Engine::GameObject(randomObstacle);
		obstacleToSpawn->addBehaviour(new Mover(_speed));
		obstacleToSpawn->transform.position = gameObject->transform.position;
		obstacleToSpawn->transform.position.x = -20.f;
		gameObject->addChild(obstacleToSpawn);
	}
}
