#include "Riser.h"

#include <vector>

#include "../Core/Time.h"
#include "../Core/GameObject.h"
#include "../Core/Transform.h"

#include "ObstacleSpawner.h"
#include "StaticSpawner.h"

Riser::Riser(float stopHeight, float speed) : _stopHeight(stopHeight),
											  _speed(speed),
											  _finished(false)
{
}

Riser *Riser::clone() const
{
	return new Riser(_stopHeight, _speed);
}

void Riser::update()
{
	Engine::Transform &transform = gameObject->transform;

	if (_finished)
	{
		return;
	}

	if (transform.position.y < _stopHeight)
	{
		transform.position += glm::vec3(0.f, 1.f, 0.f) * _speed * Engine::Time::getDeltaTime();
	}
	else
	{
		_finished = true;
		transform.position.y = _stopHeight;
		ObstacleSpawner *obstacleSpawner = gameObject->getComponent<ObstacleSpawner>();
		if (obstacleSpawner != nullptr)
		{
			obstacleSpawner->startSpawning();
		}
		std::vector<StaticSpawner *> staticSpawners = gameObject->getComponents<StaticSpawner>();
		for (StaticSpawner *staticSpawner : staticSpawners)
		{
			staticSpawner->startSpawning();
		}
	}
}
