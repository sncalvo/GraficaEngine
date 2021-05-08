#pragma once

#include "Shader.h"

namespace Engine
{
	class MaterialObject
	{
	private:
		Shader* _shader;
	public:
		MaterialObject();
		MaterialObject(Shader*);
		Shader* getShader() const;
	};
}
