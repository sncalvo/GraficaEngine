#include <glm/gtc/type_ptr.hpp>

#include "Light.h"

Light::Light(GLenum light, glm::vec4 position, glm::vec4 color)
{
	_light = light;
	_position = position;
	_color = color;
}

void Light::draw() const
{
	glEnable(_light);
	glLightfv(_light, GL_POSITION, glm::value_ptr(_position));
	glLightfv(_light, GL_DIFFUSE, glm::value_ptr(_color));
}

void Light::setColor(glm::vec4 color)
{
	_color = color;
}

void Light::setPosition(glm::vec4 position)
{
	_position = position;
}
