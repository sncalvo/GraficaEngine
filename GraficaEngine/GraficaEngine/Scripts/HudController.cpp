#include "HudController.h"

#include "../Core/Input.h"
#include "../Core/Settings.h"
#include "../Core/BaseGameObject.h"
#include "../Core/TextObject.h"
#include "../Core/Colors.h"

constexpr auto TEXT_ON = "ON";
constexpr auto TEXT_OFF = "OFF";

void HudController::update()
{
	Engine::Input &input = Engine::Input::getInstance();
	Engine::Settings &settings = Engine::Settings::getInstance();
	_changeOnOff(settings.getShowTextures(), Engine::KEY_T, "texture_value");
	_changeOnOff(settings.getIsWireframe(), Engine::KEY_Y, "wireframe_value");
	_changeOnOff(settings.getUseInterpolation(), Engine::KEY_U, "interpolation_value");
	_changeOnKey("1", Engine::KEY_1, "speed_value");
	_changeOnKey("2", Engine::KEY_2, "speed_value");
	_changeOnKey("3", Engine::KEY_3, "speed_value");
}

void HudController::_changeOnOff(bool state, Engine::KeyboardKey key, std::string valueObject)
{
	Engine::Input& input = Engine::Input::getInstance();
	if (input.getKeyDown(key))
	{
		Engine::TextObject* text = dynamic_cast<Engine::TextObject*>(gameObject->getChildWithTag(valueObject));
		if (text != nullptr)
		{
			if (state)
			{
				text->setColor(Engine::GREEN);
				text->setText(TEXT_ON);
			}
			else
			{
				text->setColor(Engine::RED);
				text->setText(TEXT_OFF);
			}
		}
	}
}

void HudController::_changeOnKey(std::string newText, Engine::KeyboardKey key, std::string valueObject)
{
	Engine::Input& input = Engine::Input::getInstance();
	if (input.getKeyDown(key))
	{
		Engine::TextObject* text = dynamic_cast<Engine::TextObject*>(gameObject->getChildWithTag(valueObject));
		if (text != nullptr)
		{
			text->setColor(Engine::BLACK);
			text->setText(newText);
		}
	}
}
