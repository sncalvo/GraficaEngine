#pragma once

#include "../Core/Behaviour.h"
#include "../Core/GameObject.h"
#include "../Core/Camera.h"
#include "../Core/Input.h"
#include "../Core/Time.h"

class SwapCameras : public Engine::Behaviour
{
private:
	int _currentIndex;
public:
	SwapCameras();
	void update();
	SwapCameras *clone() const override;
};
