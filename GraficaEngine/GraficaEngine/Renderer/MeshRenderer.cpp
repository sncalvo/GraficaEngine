#include "MeshRenderer.h"

#include <GL/glew.h>
#include <SDL/SDL_opengl.h>
#include <glm/gtc/type_ptr.hpp>

#include "../Core/Settings.h"

#include <iostream>

namespace Engine {
    std::shared_ptr<Shader> MeshRenderer::_shader{};

    MeshRenderer::MeshRenderer(std::shared_ptr<Mesh> mesh, std::shared_ptr<Mesh> meshLow, Material material, std::vector<Texture*> textures) {
        _mesh = mesh;
        _material = material;
        _textures = textures;
        _animator = NULL;
        _meshLow = meshLow;
    }

    MeshRenderer::MeshRenderer(std::shared_ptr<Mesh> mesh, std::shared_ptr<Mesh> meshLow, Material material, std::vector<Texture*> textures, Animator *animator) : MeshRenderer(mesh, meshLow, material, textures) {
        _animator = animator;
    }

    void MeshRenderer::updateAnimation(float deltaTime) {
        if (_animator) {
          _animator->UpdateAnimation(deltaTime);
        }
    }

    void MeshRenderer::draw(unsigned int depthMap, glm::vec3 cameraPos) {
        auto shader = getShader();

        glm::mat4 model = _transform->getTransformedModel();
        shader->setMatrix4f("model", glm::value_ptr(model));
        if (_animator) {
            shader->setBool("isBony", true);
            auto transforms = _animator->GetFinalBoneMatrices();
            for (int i = 0; i < transforms.size(); ++i)
                shader->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);


        } else {
            shader->setBool("isBony", false);
        }

        _setMaterialValues(depthMap);

        bool useLow = glm::distance(cameraPos, _transform->position) > 10.f;

        if (_meshLow != nullptr && useLow)
        {
            _meshLow->draw();
        }
        else
        {
            _mesh->draw();
        }
    }

    void MeshRenderer::_setMaterialValues(unsigned int depthMap) {
        unsigned int lastTexture = 0;

        glActiveTexture(GL_TEXTURE0 + lastTexture);
        glBindTexture(GL_TEXTURE_2D_ARRAY, depthMap);
        _shader->setInt("shadowMap", lastTexture);

        lastTexture += 1;

        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;

        for (unsigned int i = 0; i < _textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + lastTexture);

            std::string number;
            std::string name = _textures[i]->getType();
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);

            _shader->setBool("has_texture", true);
            _shader->setInt((name + number).c_str(), lastTexture);
            glBindTexture(GL_TEXTURE_2D, _textures[i]->ID);
            lastTexture += 1;
        }

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

    std::shared_ptr<MeshRenderer> MeshRenderer::clone() {
        return std::make_shared<MeshRenderer>(_mesh, _meshLow, _material, _textures, _animator);
    }
}
