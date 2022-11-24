#include "JumpController.h"

#include "../Core/Input.h"
#include "../Core/Time.h"
#include "../Core/GameObject.h"
#include "../Physics/Collider.h"

#include "../Utils/DebugLog.h"

#include <stdio.h>

JumpController::JumpController() : _gravity(-19.8f),
								   _velocity(0.f),
								   _mass(1.f),
								   _jumpStrength(7.f),
								   _grounded(false)
{
}

JumpController::JumpController(const JumpController* otherController) :
	_gravity(otherController->_gravity),
	_velocity(otherController->_velocity),
	_mass(otherController->_mass),
	_jumpStrength(otherController->_jumpStrength),
	_grounded(otherController->_grounded)
{
}

JumpController *JumpController::clone() const
{
	return new JumpController(this);
}

void JumpController::update()
{
	Engine::Transform &transform = gameObject->transform;

	_updateGrounded();

	if (_grounded)
	{
		_velocity = glm::vec3(0.f);
	}
	else
	{
		_updateVelocity();
	}

	if (_shouldJump())
	{
		glm::vec3 jumpDirection = glm::normalize(transform.getUp());
		_velocity = _jumpStrength * jumpDirection;
	}

	_updatePosition();

	if (transform.position.y < 0.f)
	{
		transform.position.y = 0.f;
	}
}

void JumpController::_updateGrounded()
{
	_grounded = false;
	for (Engine::Collider* collision : gameObject->getCollider()->getCollisions())
	{
		Engine::GameObject* other = collision->getGameObject();

		if (other->hasTag("ground"))
		{
			_grounded = true;
			return;
		}
	}
}

bool JumpController::_shouldJump()
{
	Engine::Input &input = Engine::Input::getInstance();

	return _grounded && input.getKeyDown(Engine::KEY_SPACE);
}

void JumpController::_updatePosition()
{
	Engine::Transform &transform = gameObject->transform;
	transform.position += _velocity * Engine::Time::getDeltaTime();
}

void JumpController::_updateVelocity()
{
	_velocity += _gravity * Engine::Time::getDeltaTime() * glm::vec3(0.f, 1.f, 0.f);
}
