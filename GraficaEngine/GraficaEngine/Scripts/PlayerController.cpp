#include "PlayerController.h"

#include "../Core/Input.h"
#include "../Core/Time.h"
#include "../Core/GameObject.h"

PlayerController::PlayerController()
{
}

void PlayerController::update()
{
	Engine::Input &input = Engine::Input::getInstance();
	Engine::Transform &transform = gameObject->transform;

	glm::vec3 movement(0.f);
	if (input.getKey(Engine::KEY_UP))
	{
		movement += transform.getForward();
	}
	if (input.getKey(Engine::KEY_DOWN))
	{
		movement -= transform.getForward();
	}
	if (input.getKey(Engine::KEY_LEFT))
	{
		movement -= transform.getRight();
	}
	if (input.getKey(Engine::KEY_RIGHT))
	{
		movement += transform.getRight();
	}

	glm::vec3 movementDirection = glm::normalize(movement);

	if (!glm::all(glm::isnan(movementDirection)))
	{
		transform.position += movementDirection * _speed * Engine::Time::getDeltaTime();
	}
}
