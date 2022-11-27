#include "PlayerController.h"

#include "../Core/Input.h"
#include "../Core/Time.h"
#include "../Core/GameObject.h"
#include "../Core/TextObject.h"
#include "../Core/SceneManager.h"

#include "../Utils/DebugLog.h"

PlayerController::PlayerController(): _speed(50.f), _canMove(true)
{
	_score = 0;
}

PlayerController *PlayerController::clone() const
{
	return new PlayerController();
}

void PlayerController::update()
{
	auto& input = Engine::Input::getInstance();
	auto& transform = gameObject->transform;
	auto* rigidBody = ((Engine::GameObject*)gameObject)->getRigidBody();

	if (_canMove)
	{
		glm::vec3 velocity(0.f);
		if (input.getKey(Engine::KEY_UP) || input.getKey(Engine::KEY_W))
		{
			velocity += transform.getForward();
		}
		if (input.getKey(Engine::KEY_DOWN) || input.getKey(Engine::KEY_S))
		{
			velocity -= transform.getForward();
		}
		if (input.getKey(Engine::KEY_LEFT) || input.getKey(Engine::KEY_A))
		{
			velocity -= transform.getRight();
		}
		if (input.getKey(Engine::KEY_RIGHT) || input.getKey(Engine::KEY_D))
		{
			velocity += transform.getRight();
		}

		glm::vec3 normalizedVelocity = glm::normalize(velocity);

		auto existingVelocity = rigidBody->getLinearVelocity();
		if (!glm::all(glm::isnan(normalizedVelocity)))
		{
			auto totalVelocity = normalizedVelocity * _speed;
			auto newVelocity = btVector3(totalVelocity.x, existingVelocity.getY(), totalVelocity.z);
			rigidBody->setLinearVelocity(newVelocity);
		}
		else
		{
			rigidBody->setLinearVelocity(btVector3(0.f, existingVelocity.getY(), 0.f));
		}
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
