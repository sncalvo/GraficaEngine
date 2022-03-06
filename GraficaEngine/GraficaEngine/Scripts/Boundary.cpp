#include "Boundary.h"

#include "../Core/GameObject.h"

Boundary::Boundary(float minX, float maxX) : _minX(minX), _maxX(maxX)
{
}

Boundary::Boundary(Boundary* otherBoundary) : _minX(otherBoundary->_minX), _maxX(otherBoundary->_maxX)
{
}

Boundary* Boundary::clone() const
{
	return new Boundary(*this);
}

void Boundary::update()
{
	Engine::Transform transform = gameObject->transform;
	if (
		transform.position.x >= _maxX ||
		transform.position.x <= _minX)
	{
		if (gameObject->hasParent())
		{
			gameObject->getScene()->deleteGameObject(gameObject);
		}
		else
		{
			gameObject->getScene()->deleteGameObject(gameObject);
		}
	}
}
