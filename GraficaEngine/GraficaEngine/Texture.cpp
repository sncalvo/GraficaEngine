#include "Texture.h"

#include "FreeImage.h"

Texture::Texture(std::string textureLocation)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(textureLocation.c_str());
	FIBITMAP* bitmap = FreeImage_Load(format, textureLocation.c_str());
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int width = FreeImage_GetWidth(bitmap);
	int height = FreeImage_GetHeight(bitmap);
	void* datos = FreeImage_GetBits(bitmap);

	glGenTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, datos);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::enableTexture() const
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);
}

void Texture::disableTexture() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}
