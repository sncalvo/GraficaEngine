#include "TimeController.h"

#include "../Core/TextObject.h"
#include "../Core/Time.h"

TimeController::TimeController()
{
	_time = 0;
	_minutes = 0;
	_seconds = 0;
}

TimeController *TimeController::clone() const
{
	return new TimeController();
}

void TimeController::update()
{
	_time += Engine::Time::getDeltaTime();
	if (_time > 1.0f)
	{
		_seconds++;
		_time = 0;
	}

	if (_seconds == 60)
	{
		_minutes++;
		_seconds = 0;
	}

	Engine::TextObject* text = dynamic_cast<Engine::TextObject*>(gameObject);
	std::string time = "";
	if (_minutes < 10)
	{
		time += "0";
	}
	time += std::to_string(_minutes) + ":";
	if (_seconds < 10)
	{
		time += "0";
	}
	time += std::to_string(_seconds);
	text->setText(time);
}
