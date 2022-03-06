#include "HomeScreenController.h"

#include "../Core/Input.h"
#include "../Core/Time.h"
#include "../Core/BaseGameObject.h"
#include "../Core/Scene.h"

HomeScreenController::HomeScreenController() : _sliding(false)
{
}

void HomeScreenController::update()
{
	Engine::Input& input = Engine::Input::getInstance();
	_sliding = _sliding ||
		input.getKeyDown(Engine::KEY_W) ||
		input.getKeyDown(Engine::KEY_A) ||
		input.getKeyDown(Engine::KEY_S) ||
		input.getKeyDown(Engine::KEY_D) ||
		input.getKeyDown(Engine::KEY_SPACE);
	if (_sliding)
	{
		gameObject->transform.position.x += 1000.f * Engine::Time::getDeltaTime();
		if (gameObject->transform.position.x >= 800.f)
		{
			gameObject->getScene()->deleteGameObject(gameObject);
		}
	}
}

HomeScreenController* HomeScreenController::clone() const
{
	return new HomeScreenController();
}
