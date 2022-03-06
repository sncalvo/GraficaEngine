#pragma once

#include "Shader.h"

namespace Engine
{
	class MaterialObject
	{
	private:
		Shader *_shader;
		glm::vec2 _textureOffset{0.f, 0.f};

	public:
		MaterialObject();
		MaterialObject(Shader *);
		Shader *getShader() const;
		void setTextureOffset(glm::vec2);
		void applyTextureOffset() const;
		glm::vec2 getTextureOffset() const;
	};
}
