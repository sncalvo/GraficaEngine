#include "Material.h"

#include <glm/gtc/type_ptr.hpp>

#include "../Utils/DebugLog.h"

namespace Engine
{
	MaterialObject::MaterialObject()
	{
	}

	void MaterialObject::setTextureOffset(glm::vec2 textureOffset)
	{
		_textureOffset = textureOffset;
	}

	void MaterialObject::applyTextureOffset(Shader *shader) const
	{	
		shader->setVec2f("texture_offset", glm::value_ptr(_textureOffset));
	}

	glm::vec2 MaterialObject::getTextureOffset() const
	{
		return _textureOffset;
	}
}
