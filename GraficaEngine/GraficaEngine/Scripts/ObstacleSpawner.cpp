#include "ObstacleSpawner.h"

#include <stdlib.h>
#include <time.h>

#include "../Core/Time.h"
#include "Mover.h"
#include "Boundary.h"
#include "Hazard.h"

ObstacleSpawner::ObstacleSpawner(std::vector<Engine::GameObject *> obstacles) :
	_obstacles(obstacles),
	_canSpawn(false)
{
	_timeToNextSpawn = Engine::Time::getTime() + rand() % 5;
	_speed = rand() % 5 + 5.f;
	_sign = ((bool)(rand() % 1)) ? -1.f : 1.f;
}

ObstacleSpawner::ObstacleSpawner(const ObstacleSpawner *otherObstacleSpawner) :
	_obstacles(otherObstacleSpawner->_obstacles),
	_canSpawn(false)
{
	_timeToNextSpawn = Engine::Time::getTime() + rand() % 5;
	_speed = rand() % 5 + 5.f;
	_sign = ((bool)(rand() % 1)) ? -1.f : 1.f;
}

ObstacleSpawner *ObstacleSpawner::clone() const
{
	return new ObstacleSpawner(this);
}

bool ObstacleSpawner::_readyToSpawn() const
{
	return Engine::Time::getTime() >= _timeToNextSpawn;
}

void ObstacleSpawner::update()
{
	if (_canSpawn && _readyToSpawn())
	{
		float spawnRate = rand() % 5 + 2;
		_timeToNextSpawn = Engine::Time::getTime() + spawnRate;

		int randomIndex = rand() % _obstacles.size();

		Engine::GameObject *randomObstacle = _obstacles[randomIndex];
		Engine::GameObject *obstacleToSpawn = new Engine::GameObject(randomObstacle);
		obstacleToSpawn->addBehaviour(new Mover(_speed));
		obstacleToSpawn->transform.position = gameObject->transform.position;
		obstacleToSpawn->transform.position.x = _sign * -20.f;
		if (_sign == -1.f)
		{
			obstacleToSpawn->transform.rotateY(180.f);
		}
		gameObject->addChild(obstacleToSpawn);
	}
}

void ObstacleSpawner::startSpawning()
{
	_canSpawn = true;
}
