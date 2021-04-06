#include "Light.h"

Light::Light(GLenum light, Point4 position, Color color)
{
	_light = light;
	_position = position;
	_color = color;
}

void Light::draw() const
{
	glEnable(_light);
	glLightfv(_light, GL_POSITION, _position._vector);
	glLightfv(_light, GL_DIFFUSE, _color._vector);
}

void Light::setColor(Color color)
{
	_color = color;
}

void Light::setPosition(Point4 position)
{
	_position = position;
}
