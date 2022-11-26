#pragma once

#include <vector>
#include <memory>

#include "Mesh.h"
#include "Shader.h"

#include "../Core/Transform.h"
#include "../Core/Animator.h"

namespace Engine {
    class MeshRenderer
    {
    public:
        MeshRenderer(std::shared_ptr<Mesh> mesh, std::shared_ptr<Mesh> meshLow, Material material, std::vector<Texture*> textures);
        MeshRenderer(std::shared_ptr<Mesh> mesh, std::shared_ptr<Mesh> meshLow, Material material, std::vector<Texture*> textures, Animator *animator);
        void draw(unsigned int depthMap, glm::vec3 cameraPos);
        void setTransform(Transform* transform) { _transform = transform; }
        void setAnimator(Animator* animator) { _animator = animator; };
        void updateAnimation(float deltaTime);
        static std::shared_ptr<Shader> getShader()
        {
            if (!_shader)
            {
                // Lazy load shader since we need opengl to be initialized first. Note, this is NOT thread safe.
                // TODO: Change all shaders to be managed by a shader manager and initialized after opengl has started.
                _shader = std::make_shared<Shader>("Assets/Shaders/default.vs", "Assets/Shaders/default.fs");
            }

            return _shader;
        }
        std::shared_ptr<MeshRenderer> clone();
    private:
        static std::shared_ptr<Shader> _shader;
        std::shared_ptr<Mesh> _mesh;
        std::shared_ptr<Mesh> _meshLow;
        Material _material;
        std::vector<Texture*> _textures;
        Transform* _transform;
        Animator* _animator;

        void _setMaterialValues(unsigned int depthMap);
    };
}
