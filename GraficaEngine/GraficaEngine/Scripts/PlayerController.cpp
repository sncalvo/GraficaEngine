#include "PlayerController.h"

#include <iostream>

PlayerController::PlayerController() {}

void PlayerController::update()
{
	Engine::Input& input = Engine::Input::getInstance();
	if (input.getKey(Engine::KEY_UP))
	{
		gameObject->transform.position -= glm::vec3(0.f, 0.f, 1.f) * _speed * Engine::Time::getDeltaTime();
	}
	else if (input.getKey(Engine::KEY_DOWN))
	{
		gameObject->transform.position += glm::vec3(0.f, 0.f, 1.f) * _speed * Engine::Time::getDeltaTime();
	}
	else if (input.getKey(Engine::KEY_LEFT))
	{
		gameObject->transform.position -= glm::vec3(1.f, 0.f, 0.f) * _speed * Engine::Time::getDeltaTime();
	}
	else if (input.getKey(Engine::KEY_RIGHT))
	{
		gameObject->transform.position += glm::vec3(1.f, 0.f, 0.f) * _speed * Engine::Time::getDeltaTime();
	}
}
