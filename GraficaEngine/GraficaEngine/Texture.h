#pragma once

#include <string>

#include "SDL_opengl.h"

class Texture
{
private:
	GLuint _textureId;
public:
	Texture(std::string textureLocation);
	void enableTexture() const;
	void disableTexture() const;
};
