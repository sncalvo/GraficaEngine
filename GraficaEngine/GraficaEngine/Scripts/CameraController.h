#pragma once

#include "../Core/Behaviour.h"
#include "../Core/GameObject.h"
#include "../Core/Camera.h"
#include "../Core/Input.h"
#include "../Core/Time.h"

class CameraController : public Engine::Behaviour
{
	void update();
};
