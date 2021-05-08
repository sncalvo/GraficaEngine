#include "Boundary.h"

#include "../Core/GameObject.h"

Boundary::Boundary(float minX, float maxX): _minX(minX), _maxX(maxX)
{}

void Boundary::update()
{
	Engine::Transform transform = gameObject->transform;
	if (
		transform.position.x >= _maxX ||
		transform.position.x <= _minX
	)
	{
		if (gameObject->hasParent())
		{
			gameObject->getParent()->deleteChild(gameObject);
		}
		else
		{
			gameObject->getScene()->deleteGameObject(gameObject);
		}
	}
}