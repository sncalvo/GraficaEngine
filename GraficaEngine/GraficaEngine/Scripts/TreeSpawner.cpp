#include "TreeSpawner.h"

TreeSpawner::TreeSpawner(Engine::GameObject *objectToSpawn) : StaticSpawner(objectToSpawn)
{
}

TreeSpawner* TreeSpawner::clone() const
{
	return new TreeSpawner(_objectToSpawn);
}

void TreeSpawner::update()
{
	if (_finished || !_canSpawn)
	{
		return;
	}

	for (int i = 0; i < 8; ++i)
	{
		Engine::GameObject* newGameObject = new Engine::GameObject(_objectToSpawn);
		newGameObject->transform.position = gameObject->transform.position + glm::vec3(.0f, 0.f, .0f);

		int positionX = -16 - i;
		int positionZ = (rand() % 3) - 1;
		newGameObject->transform.position.x += positionX;
		newGameObject->transform.position.z += positionZ;
		gameObject->addChild(newGameObject);
	}

	for (int i = 0; i < 8; ++i)
	{
		Engine::GameObject* newGameObject = new Engine::GameObject(_objectToSpawn);
		newGameObject->transform.position = gameObject->transform.position + glm::vec3(.0f, 0.f, .0f);

		int positionX = 16 + i;
		int positionZ = (rand() % 3) - 1;
		newGameObject->transform.position.x += positionX;
		newGameObject->transform.position.z += positionZ;
		gameObject->addChild(newGameObject);
	}

	_finished = true;
}
