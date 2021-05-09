#include "EndlessSpawner.h"

#include <stdlib.h>
#include <time.h>

#include "../Core/Scene.h"
#include "ObstacleSpawner.h"

EndlessSpawner::EndlessSpawner(std::map<std::string, EnvironmentWithObstacles> environments) :
	_environments(environments), _rows(new CircularBuffer<Engine::GameObject*>(10)), _currentRow(0)
{
	for (
		auto it = _environments.begin();
		it != _environments.end();
		it++
	)
	{
		_environmentNames.push_back(it->first);
	}
}

EndlessSpawner::~EndlessSpawner()
{
	delete _rows;
}

float EndlessSpawner::_getZCoordinateRow() const
{
	return FIRST_ROW - (_currentRow * SPACE_BETWEEN_ROWS);
}

bool EndlessSpawner::_nearPlayer() const
{
	float zCoordinateRow = _getZCoordinateRow();
	return (
		glm::abs(_player->transform.position.z - zCoordinateRow) <= RADIUS_TO_PLAYER
	);
}

void EndlessSpawner::update()
{
	_scene = gameObject->getScene();
	_player = _scene->getGameObjectWithTag("player");
	
	if (_player == nullptr || !_nearPlayer())
	{
		return;
	}

	srand(time(nullptr));
	int randomEnvironmentIndex = rand() % _environmentNames.size();
	std::string randomEnvironmentString = _environmentNames[randomEnvironmentIndex];

	EnvironmentWithObstacles referenceEnvironment = _environments[randomEnvironmentString];
	Engine::GameObject* environmentToSpawn = new Engine::GameObject(referenceEnvironment.first);
	environmentToSpawn->transform.position = glm::vec3(0.f, 0.f, _getZCoordinateRow());
	environmentToSpawn->transform.scale = glm::vec3(.5f);
	_scene->addGameObject(environmentToSpawn);
	environmentToSpawn->addBehaviour(new ObstacleSpawner(referenceEnvironment.second));

	_rows->put(environmentToSpawn);
	if (_rows->full())
	{
		_scene->deleteGameObject(_rows->get());
	}
	_currentRow++;
}
