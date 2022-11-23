#include "Texture.h"

#include <iostream>

#include <Freeimage/FreeImage.h>

#include "../Utils/DebugLog.h"

namespace Engine
{
	Texture::Texture(const char* path, std::string type)
	{
		_type = type;
		unsigned int texture;
		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		// set the texture wrapping/filtering options (on currently bound texture)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load and generate the texture
		FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(path);
		FIBITMAP* bitmap = FreeImage_Load(format, path);
		bitmap = FreeImage_ConvertTo32Bits(bitmap);
		int width = FreeImage_GetWidth(bitmap);
		int height = FreeImage_GetHeight(bitmap);
		void* data = FreeImage_GetBits(bitmap);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA,
				GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			if (path == nullptr)
			{
				DebugLog::error("No path");
			}
			DebugLog::error("Failed to load texture " + std::string(path));
		}
		// We might have to free the image later

		glBindTexture(GL_TEXTURE_2D, 0);
		ID = texture;
	}

	std::string Texture::getType() const
	{
		return _type;
	}

	void Texture::activateTextureAs(GLenum textureUnit) const
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ID);
	}
}
