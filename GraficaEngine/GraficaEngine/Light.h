#pragma once

#include <vector>
#include <SDL/SDL_opengl.h>

#include <glm/vec4.hpp>

#include "Drawable.h"

class Light: public Drawable
{
private:
	GLenum _light;
	glm::vec4 _position;
	glm::vec4 _color;
public:
	Light(GLenum light, glm::vec4 _position, glm::vec4 color);
	void draw() const;
	void setPosition(glm::vec4 position);
	void setColor(glm::vec4 color);
};
