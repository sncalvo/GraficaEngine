#pragma once

#include "../Core/Behaviour.h"
#include "../Core/Input.h"
#include "../Core/Light.h"

#include <string>

class HudController : public Engine::Behaviour
{
private:
	void _changeBoolSetting(bool state, std::string textObject);
	void _changeTextSetting(bool state, std::string text, std::string valueObject);
public:
	void update();
	HudController *clone() const override;
};
