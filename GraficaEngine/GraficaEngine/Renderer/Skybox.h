#pragma once

#include <vector>
#include <string>

#include "Shader.h"
#include "../Core/Light.h"

namespace Engine
{
	class Skybox
	{
	private:
		unsigned int _id, _VAO, _VBO;
		Shader *_shader;
		float* _getVertices() const;
	public:
		Skybox(std::vector<std::string> faces);
		void draw(glm::mat4 projection, glm::mat4 view, Light &light);
	};
}