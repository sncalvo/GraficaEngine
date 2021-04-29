#pragma once

#include "Shader.h"

namespace Engine
{
	class MaterialObject
	{
	private:
		Shader* _shader;
	public:
		MaterialObject(Shader*);
		Shader* getShader() const;
	};
}
