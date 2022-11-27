#include "JumpController.h"

#include "../Core/Input.h"
#include "../Core/Time.h"
#include "../Core/GameObject.h"
#include "../Physics/Collider.h"

#include "../Utils/DebugLog.h"

#include <stdio.h>

JumpController::JumpController() : _jumpStrength(5.f),
								   _grounded(false)
{
}

JumpController::JumpController(const JumpController* otherController) :
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
	auto& transform = gameObject->transform;
	auto* rigidBody = ((Engine::GameObject*)gameObject)->getRigidBody();

	_updateGrounded();

	if (_shouldJump())
	{
		glm::vec3 jumpDirection = glm::normalize(transform.getUp());
		auto velocity = _jumpStrength * btVector3(jumpDirection.x, jumpDirection.y, jumpDirection.z);
		rigidBody->setLinearVelocity(velocity);
	}
}

void JumpController::_updateGrounded()
{
	_grounded = true; // TODO: Actualizar
}

bool JumpController::_shouldJump()
{
	Engine::Input &input = Engine::Input::getInstance();

	return _grounded && input.getKeyDown(Engine::KEY_SPACE);
}