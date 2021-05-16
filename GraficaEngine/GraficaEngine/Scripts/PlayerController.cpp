#include "PlayerController.h"

#include "../Core/Input.h"
#include "../Core/Time.h"
#include "../Core/GameObject.h"

#include "../Utils/DebugLog.h"

PlayerController::PlayerController(): _speed(7.f)
{
}

void PlayerController::update()
{
	Engine::Input &input = Engine::Input::getInstance();
	Engine::Transform &transform = gameObject->transform;

	glm::vec3 movement(0.f);
	if (input.getKey(Engine::KEY_UP) || input.getKey(Engine::KEY_W))
	{
		movement += transform.getForward();
	}
	if (input.getKey(Engine::KEY_DOWN) || input.getKey(Engine::KEY_S))
	{
		movement -= transform.getForward();
	}
	if (input.getKey(Engine::KEY_LEFT) || input.getKey(Engine::KEY_A))
	{
		movement -= transform.getRight();
	}
	if (input.getKey(Engine::KEY_RIGHT) || input.getKey(Engine::KEY_D))
	{
		movement += transform.getRight();
	}

	glm::vec3 movementDirection = glm::normalize(movement);

	if (!glm::all(glm::isnan(movementDirection)))
	{
		transform.position += movementDirection * _speed * Engine::Time::getDeltaTime();
	}
}

void PlayerController::die()
{
	gameObject->getScene()->deleteGameObject(gameObject);
}
