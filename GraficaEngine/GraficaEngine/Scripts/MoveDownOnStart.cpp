#include "MoveDownOnStart.h"

#include "../Core/Input.h"
#include "../Core/Time.h"
#include "../Core/BaseGameObject.h"

MoveDownOnStart::MoveDownOnStart(float height) :
	_startAnimation(false),
	_finishAnimation(false),
	_positionMoved(0.f),
	_height(height)
{
	_smoothPosition = new SmoothAcceleratedPosition(height);
}

void MoveDownOnStart::start()
{
	_position = gameObject->transform.position.y;
}

void MoveDownOnStart::update()
{
	Engine::Input& input = Engine::Input::getInstance();
	_startAnimation = _startAnimation ||
		input.getKeyDown(Engine::KEY_W) ||
		input.getKeyDown(Engine::KEY_A) ||
		input.getKeyDown(Engine::KEY_S) ||
		input.getKeyDown(Engine::KEY_D) ||
		input.getKeyDown(Engine::KEY_SPACE);
	if (_startAnimation && !_finishAnimation)
	{
		float currentMove = _smoothPosition->getPosition(Engine::Time::getDeltaTime());
		gameObject->transform.position.y = _position - currentMove;
		_finishAnimation = currentMove >= _height;
	}
}

MoveDownOnStart* MoveDownOnStart::clone() const
{
	return new MoveDownOnStart(_height);
}

MoveDownOnStart::~MoveDownOnStart()
{
	delete _smoothPosition;
}
