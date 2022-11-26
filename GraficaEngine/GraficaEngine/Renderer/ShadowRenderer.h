#pragma once

#include <memory>

#include "./Mesh.h"
#include "./Shader.h"

#include "../Core/Transform.h"
#include "../Core/Animator.h"

namespace Engine {
    class ShadowRenderer
    {
    public:
        ShadowRenderer(std::shared_ptr<Mesh> mesh, std::shared_ptr<Mesh> meshLow);
        ShadowRenderer(std::shared_ptr<Mesh> mesh);
        ShadowRenderer(std::shared_ptr<Mesh> mesh, std::shared_ptr<Mesh> meshLow, Animator *animator);
        std::shared_ptr<ShadowRenderer> clone()
        {
            return std::make_shared<ShadowRenderer>(_mesh, _meshLow, _animator);
        }
        void draw(glm::vec3 cameraPos);
        void setTransform(Transform* transform) { _transform = transform; }
        void setAnimator(Animator* animator) { _animator = animator; };
        static std::shared_ptr<Shader> getShader();

    private:
        static std::shared_ptr<Shader> _shader;
        std::shared_ptr<Mesh> _mesh;
        std::shared_ptr<Mesh> _meshLow;
        Transform* _transform;
        Animator* _animator;
    };
}
