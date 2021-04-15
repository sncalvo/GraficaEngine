#pragma once

#include "Shader.h"

class MaterialObject
{
private:
	Shader* _shader;
public:
	MaterialObject(Shader*);
	Shader* getShader() const;
};