#include "PlayerController.h"

#include "../Core/Input.h"
#include "../Core/Time.h"
#include "../Core/GameObject.h"
#include "../Core/TextObject.h"
#include "../Core/SceneManager.h"

#include "../Utils/DebugLog.h"

PlayerController::PlayerController(): _speed(7.f), _canMove(true)
{
	_score = 0;
}

PlayerController *PlayerController::clone() const
{
	return new PlayerController();
}

void PlayerController::update()
{
	Engine::Input& input = Engine::Input::getInstance();
	Engine::Transform& transform = gameObject->transform;

	if (_canMove)
	{
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

	if (transform.position.x <= -14.f)
	{
		transform.position.x = -14.f;
	}
	else if (transform.position.x >= 14.f)
	{
		transform.position.x = 14.f;
	}

	if (transform.position.z >= 5.f)
	{
		transform.position.z = 5.f;
	}
}

void PlayerController::die()
{
	Engine::SceneManager &sceneManager = Engine::SceneManager::getInstance();
	sceneManager.setShouldRestart(true);
}

void PlayerController::increaseScore(int amount)
{
	_score += amount;
	Engine::TextObject* text = dynamic_cast<Engine::TextObject*>(gameObject->getScene()->getGameObjectWithTag("score_value"));
	text->setText(std::to_string(_score));
}

void PlayerController::setCanMove(bool canMove)
{
	_canMove = canMove;
}
