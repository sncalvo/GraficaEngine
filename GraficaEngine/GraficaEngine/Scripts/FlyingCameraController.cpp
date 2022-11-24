#include "FlyingCameraController.h"
#include <iostream>

FlyingCameraController *FlyingCameraController::clone() const
{
	return new FlyingCameraController();
}

void FlyingCameraController::update()
{
	Engine::Input& input = Engine::Input::getInstance();

	int movementX, movementY;
	std::tie(movementX, movementY) = input.getMouseMovement();

	Engine::Camera* camera = static_cast<Engine::Camera*>(gameObject);

	camera->processMouseMovement(float(movementX) * Engine::Time::getDeltaTime(), float(movementY) * Engine::Time::getDeltaTime());
	if (input.getKey(Engine::KEY_UP) || input.getKey(Engine::KEY_W))
	{
		camera->processKeyboard(Engine::Camera_Movement::FORWARD, Engine::Time::getDeltaTime());
	}
	if (input.getKey(Engine::KEY_DOWN) || input.getKey(Engine::KEY_S))
	{
		camera->processKeyboard(Engine::Camera_Movement::BACKWARD, Engine::Time::getDeltaTime());
	}
	if (input.getKey(Engine::KEY_LEFT) || input.getKey(Engine::KEY_A))
	{
		camera->processKeyboard(Engine::Camera_Movement::LEFT, Engine::Time::getDeltaTime());
	}
	if (input.getKey(Engine::KEY_RIGHT) || input.getKey(Engine::KEY_D))
	{
		camera->processKeyboard(Engine::Camera_Movement::RIGHT, Engine::Time::getDeltaTime());
	}
	if (input.getKey(Engine::KEY_RIGHT_SHIFT))
	{
		camera->processKeyboard(Engine::Camera_Movement::UP, Engine::Time::getDeltaTime());
	}
	if (input.getKey(Engine::KEY_RIGHT_CTRL))
	{
		camera->processKeyboard(Engine::Camera_Movement::DOWN, Engine::Time::getDeltaTime());
	}
}
