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
		Engine::Collider *collider = new Engine::Collider(glm::vec3(-1.f), glm::vec3(1.f));
		obstacleToSpawn->setCollider(collider);
		obstacleToSpawn->addBehaviour(new Mover(_speed));
		obstacleToSpawn->addBehaviour(new Boundary(-30.f, 10.f));
		obstacleToSpawn->addBehaviour(new Hazard());
		obstacleToSpawn->addTag("hazard");
		obstacleToSpawn->transform.position = gameObject->transform.position;
		obstacleToSpawn->transform.position.x = -20.f;
		gameObject->addChild(obstacleToSpawn);
	}
}
