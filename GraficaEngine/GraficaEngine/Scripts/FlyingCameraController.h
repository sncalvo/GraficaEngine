#pragma once

#include "../Core/Behaviour.h"
#include "../Core/Camera.h"
#include "../Core/Input.h"
#include "../Core/Time.h"

class FlyingCameraController : public Engine::Behaviour
{
	void update();
};
