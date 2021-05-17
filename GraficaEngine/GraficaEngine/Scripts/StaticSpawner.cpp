#include "StaticSpawner.h"

#include <stdlib.h>

StaticSpawner::StaticSpawner(Engine::GameObject* objectToSpawn) :
	_finished(false)
{
	_objectToSpawn = objectToSpawn;
}

void StaticSpawner::update() {
	if (_finished || !_canSpawn)
	{
		return;
	}

	int random = rand() % 5;
	if (random == 0)
	{
		Engine::GameObject* newGameObject = new Engine::GameObject(_objectToSpawn);
		newGameObject->transform.position = gameObject->transform.position + glm::vec3(.0f, 1.f, .0f);
		int position = (rand() % 25) - 11;
		newGameObject->transform.position.x += position;
		gameObject->addChild(newGameObject);
		_finished = true;
	}
}

void StaticSpawner::startSpawning()
{
	_canSpawn = true;
}

StaticSpawner *StaticSpawner::clone() const
{
	return new StaticSpawner(_objectToSpawn);
}
