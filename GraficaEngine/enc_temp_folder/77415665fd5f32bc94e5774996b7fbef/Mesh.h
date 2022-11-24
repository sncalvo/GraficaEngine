#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "Texture.h"
#define MAX_BONE_INFLUENCE 4

namespace Engine
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 textureCoordinates;
		// tangent
		glm::vec3 Tangent;
		// bitangent
		glm::vec3 Bitangent;

		//bone indexes which will influence this vertex
		int m_BoneIDs[MAX_BONE_INFLUENCE];
		//weights from each bone
		float m_Weights[MAX_BONE_INFLUENCE];
	};

	struct Material
	{
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

		unsigned int VAO, VBO, EBO;

		void setupMesh();

	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> _indices);
		void draw() const;
	};
}
