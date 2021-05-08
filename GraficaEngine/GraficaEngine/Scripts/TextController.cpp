#include "TextController.h"
#include "../Core/Input.h"
#include "../Core/TextObject.h"

void TextController::update()
{
	Engine::Input &input = Engine::Input::getInstance();
	Engine::TextObject* text = dynamic_cast<Engine::TextObject*>(gameObject);
	if (input.getKey(Engine::KEY_UP))
	{
		text->setText("Arriba");
	}
	else if (input.getKey(Engine::KEY_DOWN))
	{
		text->setText("Abajo");
	}
	else if (input.getKey(Engine::KEY_RIGHT))
	{
		text->setText("Derecha");
	}
	else if (input.getKey(Engine::KEY_LEFT))
	{
		text->setText("Izquierda");
	}
	else
	{
		text->setText("Quieto");
	}
}
