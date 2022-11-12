#include "MeshRenderer.h"

#include <GL/glew.h>
#include <SDL/SDL_opengl.h>
#include <glm/gtc/type_ptr.hpp>

#include "../Core/Settings.h"

#include <iostream>

namespace Engine {
    std::shared_ptr<Shader> MeshRenderer::_shader{};

    MeshRenderer::MeshRenderer(std::shared_ptr<Mesh> mesh, Material material, std::vector<Texture> textures) {
        _mesh = mesh;
        _material = material;
        _textures = textures;
    }

    void MeshRenderer::draw(glm::vec3 viewPosition) {
        auto shader = getShader();

        glm::mat4 model = _transform->getTransformedModel();
        shader->setMatrix4f("model", glm::value_ptr(model));
        shader->setVec3f("viewPos", glm::value_ptr(viewPosition));

        _setMaterialValues();

        _mesh->draw();
    }

    void MeshRenderer::_setMaterialValues() {
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

            _shader->setBool("has_texture", true);
            _shader->setFloat((name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, _textures[i].ID);
        }
        glActiveTexture(GL_TEXTURE0);

        Settings& settings = Settings::getInstance();

        if (_textures.size() == 0 || !settings.getShowTextures())
        {
            _shader->setBool("has_texture", false);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        _shader->setBool("is_flat", !settings.getUseInterpolation());

        _shader->setVec3f("material.ambient", glm::value_ptr(_material.ambient));
        _shader->setVec3f("material.diffuse", glm::value_ptr(_material.diffuse));
        _shader->setVec3f("material.specular", glm::value_ptr(_material.specular));
        _shader->setFloat("material.shininess", _material.shininess);
    }
}
