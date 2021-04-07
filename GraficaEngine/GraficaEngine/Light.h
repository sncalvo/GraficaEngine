#pragma once

#include <vector>

#include "SDL_openGL.h"
#include "Math.h"
#include "Drawable.h"

class Light: public Drawable
{
private:
	GLenum _light;
	Point4 _position;
	Color _color;
public:
	Light(GLenum light, Point4 _position, Color color);
	void draw() const;
	void setPosition(Point4 position);
	void setColor(Color color);
};
