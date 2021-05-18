#include "GameKeysHomeController.h"

#include "../Core/Input.h"
#include "../Core/Time.h"
#include "../Core/BaseGameObject.h"
#include "../Core/Scene.h"
#include "../Core/TextObject.h"
#include "../Core/Colors.h"

GameKeysHomeController::GameKeysHomeController() : _time(0.f), _switchColor(true), _sliding(false)
{
}

void GameKeysHomeController::update()
{
	_time += Engine::Time::getDeltaTime();
	Engine::Input& input = Engine::Input::getInstance();
	_sliding = _sliding ||
		input.getKeyDown(Engine::KEY_W) ||
		input.getKeyDown(Engine::KEY_A) ||
		input.getKeyDown(Engine::KEY_S) ||
		input.getKeyDown(Engine::KEY_D) ||
		input.getKeyDown(Engine::KEY_SPACE);
	if (_sliding)
	{
		gameObject->transform.position.x -= 1000.f * Engine::Time::getDeltaTime();
		if (gameObject->transform.position.x <= -500.f)
		{
			gameObject->getScene()->deleteGameObject(gameObject);
		}
	}
	if (_time >= .7f)
	{
		Engine::TextObject* text = dynamic_cast<Engine::TextObject*>(gameObject->getChildWithTag("text"));
		_time = 0;
		if (_switchColor)
		{
			text->setColor(Engine::BLACK);
		}
		else
		{
			text->setColor(Engine::WHITE);
		}
		_switchColor = !_switchColor;
	}
}

GameKeysHomeController* GameKeysHomeController::clone() const
{
	return new GameKeysHomeController();
}
