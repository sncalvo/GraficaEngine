#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "Texture.h"
#include "Shader.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoordinates;
};

class Mesh
{
private:
	std::vector<Vertex> _vertices;
	std::vector<unsigned int> _indices;
	std::vector<Texture> _textures;

	unsigned int VAO, VBO, EBO;

	void setupMesh();
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> _indices, std::vector<Texture> texture);
	void draw(Shader &shader);
};
