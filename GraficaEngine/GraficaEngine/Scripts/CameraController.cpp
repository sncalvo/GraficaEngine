#include <vector>

#include "CameraController.h"

void CameraController::update()
{
	Engine::Input& input = Engine::Input::getInstance();

	int movementX, movementY;
	std::tie(movementX, movementY) = input.getMouseMovement();

	Engine::Camera* camera = gameObject->getScene()->getCamera();

	camera->processMouseMovement(float(movementX) * Engine::Time::getDeltaTime(), float(movementY) * Engine::Time::getDeltaTime());
	if (input.getKey(Engine::KEY_W))
	{
		camera->processKeyboard(Engine::Camera_Movement::FORWARD, Engine::Time::getDeltaTime());
	}
	if (input.getKey(Engine::KEY_S))
	{
		camera->processKeyboard(Engine::Camera_Movement::BACKWARD, Engine::Time::getDeltaTime());
	}
	if (input.getKey(Engine::KEY_A))
	{
		camera->processKeyboard(Engine::Camera_Movement::LEFT, Engine::Time::getDeltaTime());
	}
	if (input.getKey(Engine::KEY_D))
	{
		camera->processKeyboard(Engine::Camera_Movement::RIGHT, Engine::Time::getDeltaTime());
	}
}
