#pragma once

#include "../Core/Behaviour.h"
#include "../Core/Input.h"

#include <string>

class HudController : public Engine::Behaviour
{
private:
	void _changeOnOff(bool state, Engine::KeyboardKey key, std::string textObject);
	void _changeOnKey(std::string newText, Engine::KeyboardKey, std::string valueObject);
public:
	void update();
	HudController *clone() const override;
};
