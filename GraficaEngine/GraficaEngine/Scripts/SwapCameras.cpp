#include <vector>

#include <iostream>

#include "SwapCameras.h"

void SwapCameras::update()
{
	Engine::Input& input = Engine::Input::getInstance();

	Engine::Scene* scene = gameObject->getScene();

	if (input.getKeyDown(Engine::KEY_V))
	{
		_currentIndex = (_currentIndex + 1) % scene->getCameraNames().size();
		
		std::string newCameraName = scene->getCameraNames()[_currentIndex];
		scene->setActiveCamera(newCameraName);
	}
}
