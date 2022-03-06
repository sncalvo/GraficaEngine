#include "RiverMover.h"

#include "../Core/GameObject.h"
#include "../Core/Time.h"

#include <stdio.h>

RiverMover::RiverMover(float speed)
{
	_movementSpeed = speed;
}

RiverMover* RiverMover::clone() const
{
	return new RiverMover(_movementSpeed);
}

void RiverMover::update()
{
	Engine::MaterialObject &material = dynamic_cast<Engine::GameObject*>(gameObject)->getMaterial();

	glm::vec2 offset = glm::vec2(0.f, 1.f) + _movementSpeed * Engine::Time::getDeltaTime();
	material.setTextureOffset(material.getTextureOffset() + offset);
}
