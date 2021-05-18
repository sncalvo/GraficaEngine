#include "HudController.h"

#include "../Core/Input.h"
#include "../Core/Settings.h"
#include "../Core/BaseGameObject.h"
#include "../Core/TextObject.h"
#include "../Core/Colors.h"

constexpr auto TEXT_ON = "ON";
constexpr auto TEXT_OFF = "OFF";

HudController *HudController::clone() const
{
	return new HudController();
}

void HudController::update()
{
	Engine::Settings &settings = Engine::Settings::getInstance();
	_changeBoolSetting(settings.getShowTextures(), "texture_value");
	_changeBoolSetting(settings.getIsWireframe(), "wireframe_value");
	_changeBoolSetting(settings.getUseInterpolation(), "interpolation_value");
	_changeTextSetting(settings.getGameSpeedEnum() == Engine::GameSpeed::SLOW, "1", "speed_value");
	_changeTextSetting(settings.getGameSpeedEnum() == Engine::GameSpeed::NORMAL, "2", "speed_value");
	_changeTextSetting(settings.getGameSpeedEnum() == Engine::GameSpeed::FAST, "3", "speed_value");
	_changeTextSetting(settings.getDayTime() == Engine::DayTime::AFTERNOON, "Tarde", "light_value");
	_changeTextSetting(settings.getDayTime() == Engine::DayTime::MIDDAY, "Mediodía", "light_value");
	_changeTextSetting(settings.getDayTime() == Engine::DayTime::MORNING, "Mañana", "light_value");
	_changeTextSetting(settings.getDayTime() == Engine::DayTime::NIGHT, "Noche", "light_value");
}

void HudController::_changeBoolSetting(bool state, std::string valueObject)
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

void HudController::_changeTextSetting(bool state, std::string textValue, std::string valueObject)
{
	Engine::TextObject* text = dynamic_cast<Engine::TextObject*>(gameObject->getChildWithTag(valueObject));
	if (text != nullptr && state)
	{
		text->setText(textValue);
	}
}
