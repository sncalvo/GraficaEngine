#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "Texture.h"
#include "Shader.h"

namespace Engine
{
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 textureCoordinates;
	};

	struct Material {
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;
	};

	class Mesh
	{
	private:
		std::vector<Vertex> _vertices;
		std::vector<unsigned int> _indices;
		std::vector<Texture> _textures;
		Material _material;

		unsigned int VAO, VBO, EBO;

		void setupMesh();
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> _indices, std::vector<Texture> texture, Material material);
		void draw(Shader &shader) const;
	};
}