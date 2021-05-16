#include "CoinController.h"

#include "../Core/BaseGameObject.h"
#include "../Core/GameObject.h"
#include "../Core/Time.h"
#include "../Physics/Collider.h"
#include "PlayerController.h"

constexpr auto COIN_POINTS = 10;

CoinController::CoinController()
{
	_dying = false;
	_dyingCountdown = .5f;
}

void CoinController::update()
{
	float angle;
	if (!_dying)
	{
		angle = 200.f;
		for (Engine::Collider *other : gameObject->getCollider()->getCollisions())
		{
			if (other->getGameObject()->hasTag("player")) {
				PlayerController* pc = other->getGameObject()->getComponent<PlayerController>();
				pc->increaseScore(COIN_POINTS);
				angle = 500.f;
				_dying = true;
				break;
			}
		}
	}
	else
	{
		angle = 500.f;
		float levitate = 20.f;
		_dyingCountdown -= Engine::Time::getDeltaTime();
		gameObject->transform.position += glm::vec3(0.f, levitate * Engine::Time::getDeltaTime(), 0.f);
	}
	gameObject->transform.rotateY(angle * Engine::Time::getDeltaTime());
	if (_dyingCountdown <= 0)
	{
		gameObject->getScene()->deleteGameObject(gameObject);
	}
}

CoinController* CoinController::clone() const
{
	return new CoinController();
}
