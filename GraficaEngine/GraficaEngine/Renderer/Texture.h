#pragma once

#include <GL/glew.h>
#include <SDL/SDL_opengl.h>

#include <string>

namespace Engine
{
	class Texture
	{
	private:
		std::string _type;
	public:
		unsigned int ID;
		std::string getType() const;
		Texture(const char* texturePath, std::string type);
		void activateTextureAs(GLenum textureUnit) const;
	};
}
