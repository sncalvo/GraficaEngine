#include "Material.h"

namespace Engine
{
	MaterialObject::MaterialObject(Shader* shader)
	{
		_shader = shader;
	}

	Shader* MaterialObject::getShader() const
	{
		return _shader;
	}
}