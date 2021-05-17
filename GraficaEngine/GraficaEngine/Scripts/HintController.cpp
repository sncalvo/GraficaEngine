#include "HintController.h"

#include "../Core/Time.h"
#include "../Core/BaseGameObject.h"
#include "../Core/Scene.h"

HintController::HintController(float hintDuration, float xDisplacement) : _hintDuration(hintDuration), _xDisplacement(xDisplacement), _direction(-1.f), _position(0.f), _sliding(true)
{}

HintController::HintController(const HintController *otherController) :
	_hintDuration(otherController->_hintDuration),
	_direction(otherController->_direction),
	_xDisplacement(otherController->_xDisplacement),
	_position(otherController->_position),
	_sliding(otherController->_sliding)
{
}

HintController *HintController::clone() const
{
	return new HintController(this);
}

void HintController::update()
{
	if (_sliding)
	{
		gameObject->transform.position += glm::vec3(_xDisplacement * _direction * Engine::Time::getDeltaTime(), 0.f, 0.f);
		_position += _xDisplacement * Engine::Time::getDeltaTime();
		if (_position >= _xDisplacement)
		{
			_sliding = false;
			_position = 0;
			_direction *= -1.f;
			if (_hintDuration <= 0)
			{
				gameObject->getScene()->deleteGameObject(gameObject);
			}
		}
	}
	else
	{
		_hintDuration -= Engine::Time::getDeltaTime();
		_sliding = _hintDuration <= 0;
	}
}
