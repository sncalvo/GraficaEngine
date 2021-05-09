#include "Hazard.h"

#include "../Physics/Collider.h"
#include "../Core/GameObject.h"

#include "PlayerController.h"

void Hazard::update()
{
	_checkCollision();
}

void Hazard::_checkCollision()
{
	for (Engine::Collider *collision : gameObject->getCollider()->getCollisions())
	{
		Engine::GameObject *gameObject = collision->getGameObject();
		if (gameObject->hasTag("player"))
		{
			PlayerController *playerController = gameObject->getComponent<PlayerController>();
			playerController->die();
		}
	}
}
