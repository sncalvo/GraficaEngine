#include "StaticSpawner.h"

#include <stdlib.h>

StaticSpawner::StaticSpawner(Engine::GameObject* objectToSpawn)
{
	_objectToSpawn = objectToSpawn;
}

void StaticSpawner::start() {
	int random = rand() % 5;
	if (random == 0)
	{
		Engine::GameObject* newGameObject = new Engine::GameObject(_objectToSpawn);
		newGameObject->transform.position = gameObject->transform.position + glm::vec3(.0f, 1.f, .0f);
		int position = (rand() % 25) - 11;
		newGameObject->transform.position.x += position;
		gameObject->addChild(newGameObject);
	}
}

StaticSpawner *StaticSpawner::clone() const
{
	return new StaticSpawner(_objectToSpawn);
}
