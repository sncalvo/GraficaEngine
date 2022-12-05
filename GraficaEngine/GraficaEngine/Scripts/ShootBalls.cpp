#include "ShootBalls.h"

#include "../Core/Input.h"

ShootBalls::ShootBalls(Engine::GameObject* ballPrefab) : _ballPrefab(ballPrefab), _speed(10.f)
{}

ShootBalls* ShootBalls::clone() const
{
	return new ShootBalls(_ballPrefab);
}

void ShootBalls::update()
{
	auto& input = Engine::Input::getInstance();
	auto* scene = gameObject->getScene();
	auto& transform = gameObject->transform;

	if (input.getKey(Engine::KEY_SPACE))
	{
		auto* ball = new Engine::GameObject(_ballPrefab);

		btTransform ballTransform;
		ballTransform.setIdentity();
		auto ballPosition = transform.position + transform.getUp() + transform.getForward();
		ballTransform.setOrigin(btVector3(
			ballPosition.x,
			ballPosition.y,
			ballPosition.z
		));
		ball->getRigidBody()->setWorldTransform(ballTransform);
		
		auto ballVelocity = transform.getForward() * _speed;
		ball->getRigidBody()->setLinearVelocity(btVector3(
			ballVelocity.x,
			ballVelocity.y,
			ballVelocity.z
		));
		
		scene->addGameObject(ball);
	}
}