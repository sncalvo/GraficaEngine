#pragma once

#include <vector>
#include <memory>

#include "Mesh.h"
#include "Shader.h"

#include "../Core/Transform.h"

namespace Engine {
    class MeshRenderer
    {
    public:
        MeshRenderer(std::shared_ptr<Mesh> mesh, Material material, std::vector<Texture*> textures);
        void draw(unsigned int depthMap);
        void setTransform(Transform* transform) { _transform = transform; }
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
        std::shared_ptr<MeshRenderer> clone()
        {
            return std::make_shared<MeshRenderer>(_mesh, _material, _textures);
        }

    private:
        static std::shared_ptr<Shader> _shader;
        std::shared_ptr<Mesh> _mesh;
        Material _material;
        std::vector<Texture*> _textures;
        Transform* _transform;

        void _setMaterialValues(unsigned int depthMap);
    };
}
