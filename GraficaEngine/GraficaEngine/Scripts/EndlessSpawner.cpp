#include "EndlessSpawner.h"

#include <stdlib.h>
#include <time.h>

#include "../Core/Scene.h"
#include "ObstacleSpawner.h"
#include "Riser.h"

EndlessSpawner::EndlessSpawner(std::vector<Environment> environments) : _environments(environments), _rows(new CircularBuffer<Engine::GameObject *>(30)), _currentRow(0)
{
}

EndlessSpawner::~EndlessSpawner()
{
	delete _rows;
}

EndlessSpawner::EndlessSpawner(const EndlessSpawner *otherEndlessSpawner) : _rows(new CircularBuffer<Engine::GameObject *>(30)),
																			_currentRow(0)
{
	for (Environment environment : otherEndlessSpawner->_environments)
	{
		_environments.push_back(environment->clone());
	}
}

EndlessSpawner *EndlessSpawner::clone() const
{
	return new EndlessSpawner(this);
}

float EndlessSpawner::_getZCoordinateRow() const
{
	return FIRST_ROW - (_currentRow * SPACE_BETWEEN_ROWS);
}

bool EndlessSpawner::_nearPlayer() const
{
	float zCoordinateRow = _getZCoordinateRow();
	return (
		glm::abs(_player->transform.position.z - zCoordinateRow) <= RADIUS_TO_PLAYER);
}

void EndlessSpawner::update()
{
	_scene = gameObject->getScene();
	_player = _scene->getGameObjectWithTag("player");

	if (_player == nullptr || !_nearPlayer())
	{
		return;
	}

	int randomEnvironmentIndex = rand() % _environments.size();

	Environment referenceEnvironment = _environments[randomEnvironmentIndex];
	Environment environmentToSpawn = new Engine::GameObject(referenceEnvironment);
	environmentToSpawn->transform.position = glm::vec3(0.f, -5.f, _getZCoordinateRow());
	environmentToSpawn->transform.scale = glm::vec3(1.f, 1.0, .5f); // TODO: Do it in main once transforms are copied
	environmentToSpawn->addBehaviour(new Riser());
	_scene->addGameObject(environmentToSpawn);

	_rows->put(environmentToSpawn);
	if (_rows->full())
	{
		_scene->deleteGameObject(_rows->get());
	}
	_currentRow++;
}
