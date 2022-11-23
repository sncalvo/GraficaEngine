#include "ShadowRenderer.h"

#include <glm/gtc/type_ptr.hpp>

namespace Engine {
    std::shared_ptr<Shader> ShadowRenderer::_shader{};

    ShadowRenderer::ShadowRenderer(std::shared_ptr<Mesh> mesh) : _mesh(mesh), _transform(nullptr), _animator(NULL) {}
    ShadowRenderer::ShadowRenderer(std::shared_ptr<Mesh> mesh, Animator *animator) : _mesh(mesh), _transform(nullptr), _animator(animator) {}

    void ShadowRenderer::draw()
    {
        if (!_transform)
        {
            return;
        }

        auto shader = getShader();
        glm::mat4 model = _transform->getTransformedModel();
        shader->setMatrix4f("model", glm::value_ptr(model));
        if(_animator) {
            shader->setBool("isBony", true);
            auto transforms = _animator->GetFinalBoneMatrices();
            for (int i = 0; i < transforms.size(); ++i)
                shader->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);


        } else {
            shader->setBool("isBony", false);
        }

        _mesh->draw();
    }

    std::shared_ptr<Shader> ShadowRenderer::getShader()
    {
        if (!_shader)
        {
            // Lazy load shader since we need opengl to be initialized first. Note, this is NOT thread safe.
            // TODO: Change all shaders to be managed by a shader manager and initialized after opengl has started.
            _shader = std::make_shared<Shader>("Assets/Shaders/depthTest.vs", "Assets/Shaders/depthTest.gs", "Assets/Shaders/depthTest.fs");
        }

        return _shader;
    }
}
