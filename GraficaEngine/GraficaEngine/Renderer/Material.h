#pragma once

#include "Shader.h"

namespace Engine
{
	class MaterialObject
	{
	private:
		glm::vec2 _textureOffset{0.f, 0.f};

	public:
		MaterialObject();
		void setTextureOffset(glm::vec2);
		void applyTextureOffset(Shader *) const;
		glm::vec2 getTextureOffset() const;
	};
}
