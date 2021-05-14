#include "FlyingCameraController.h"

void FlyingCameraController::update()
{
	Engine::Input& input = Engine::Input::getInstance();

	int movementX, movementY;
	std::tie(movementX, movementY) = input.getMouseMovement();

	Engine::Camera* camera = static_cast<Engine::Camera*>(gameObject);

	camera->processMouseMovement(float(movementX) * Engine::Time::getDeltaTime(), float(movementY) * Engine::Time::getDeltaTime());
	if (input.getKey(Engine::KEY_I))
	{
		camera->processKeyboard(Engine::Camera_Movement::FORWARD, Engine::Time::getDeltaTime());
	}
	if (input.getKey(Engine::KEY_K))
	{
		camera->processKeyboard(Engine::Camera_Movement::BACKWARD, Engine::Time::getDeltaTime());
	}
	if (input.getKey(Engine::KEY_J))
	{
		camera->processKeyboard(Engine::Camera_Movement::LEFT, Engine::Time::getDeltaTime());
	}
	if (input.getKey(Engine::KEY_L))
	{
		camera->processKeyboard(Engine::Camera_Movement::RIGHT, Engine::Time::getDeltaTime());
	}
}
