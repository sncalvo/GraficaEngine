#include <GL/glew.h>
#include <SDL/SDL_opengl.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "../Core/Settings.h"
#include "Mesh.h"

namespace Engine
{
    Mesh::Mesh(
        std::vector<Vertex> vertices,
        std::vector<unsigned int> indices,
        std::vector<Texture> textures,
        Material material
    ):
        _vertices(vertices),
        _indices(indices),
        _textures(textures),
        _material(material)
    {
        setupMesh();
    }

    void Mesh::setupMesh()
    {
        glGenVertexArrays(1, &VAO);

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));

        glBindVertexArray(0);
    }

    void Mesh::draw(Shader &shader) const
    {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        for (unsigned int i = 0; i < _textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);

            std::string number;
            std::string name = _textures[i].getType();
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);

            shader.setBool("has_texture", true);
            shader.setFloat((name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, _textures[i].ID);
        }
        glActiveTexture(GL_TEXTURE0);

        Settings &settings = Settings::getInstance();

        if (_textures.size() == 0 || !settings.getShowTextures())
        {
            shader.setBool("has_texture", false);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        shader.setBool("is_flat", !settings.getUseInterpolation());

        shader.setVec3f("material.ambient", glm::value_ptr(_material.ambient));
        shader.setVec3f("material.diffuse", glm::value_ptr(_material.diffuse));
        shader.setVec3f("material.specular", glm::value_ptr(_material.specular));
        shader.setFloat("material.shininess", _material.shininess);

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
