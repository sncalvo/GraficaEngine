#include <vector>

#include <iostream>

#include "SwapCameras.h"

SwapCameras::SwapCameras() : _currentIndex(0)
{
}

SwapCameras *SwapCameras::clone() const
{
	return new SwapCameras();
}

void SwapCameras::update()
{
	Engine::Input& input = Engine::Input::getInstance();

	Engine::Scene* scene = gameObject->getScene();

	Engine::BaseGameObject* player = scene->getGameObjectWithTag("player");

	if (input.getKeyDown(Engine::KEY_V))
	{
		_currentIndex = (_currentIndex + 1) % scene->getCameraNames().size();
		
		std::string newCameraName = scene->getCameraNames()[_currentIndex];
		if (newCameraName == "default" || newCameraName == "centeredFixed")
		{
			player->transform.lookAt(glm::vec3(0.f, 0.f, -1.f));
		}
		scene->setActiveCamera(newCameraName);
	}
}
