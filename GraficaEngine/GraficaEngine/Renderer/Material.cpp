#include "Material.h"

#include <glm/gtc/type_ptr.hpp>

#include "../Utils/DebugLog.h"

namespace Engine
{
	MaterialObject::MaterialObject(): _shader(nullptr)
	{
	}

	MaterialObject::MaterialObject(Shader* shader): _shader(shader)
	{
	}

	Shader* MaterialObject::getShader() const
	{
		return _shader;
	}

	void MaterialObject::setTextureOffset(glm::vec2 textureOffset)
	{
		_textureOffset = textureOffset;
	}

	void MaterialObject::applyTextureOffset() const
	{	
		_shader->setVec2f("texture_offset", glm::value_ptr(_textureOffset));
	}

	glm::vec2 MaterialObject::getTextureOffset() const
	{
		return _textureOffset;
	}
}
