#include "HintController.h"

#include "../Core/Time.h"
#include "../Core/BaseGameObject.h"
#include "../Core/Scene.h"

HintController::HintController(float hintDuration, float xDisplacement) : _hintDuration(hintDuration), _xDisplacement(xDisplacement), _direction(-1.f), _position(0.f), _sliding(true)
{
	_smoothPosition = new SmoothAcceleratedPosition(xDisplacement);
}

HintController::HintController(const HintController *otherController) :
	_hintDuration(otherController->_hintDuration),
	_direction(otherController->_direction),
	_xDisplacement(otherController->_xDisplacement),
	_position(otherController->_position),
	_sliding(otherController->_sliding)
{
	_smoothPosition = new SmoothAcceleratedPosition(otherController->_xDisplacement);
}

void HintController::start()
{
	_position = gameObject->transform.position.x;
}

HintController *HintController::clone() const
{
	return new HintController(this);
}

void HintController::update()
{
	if (_sliding)
	{
		float currentMove = _smoothPosition->getPosition(Engine::Time::getDeltaTime());
		gameObject->transform.position.x = _position + (_direction * currentMove);
		if (currentMove >= _xDisplacement)
		{
			_sliding = false;
			_position = gameObject->transform.position.x;
			_direction *= -1.f;
			delete _smoothPosition;
			_smoothPosition = new SmoothAcceleratedPosition(_xDisplacement);
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
