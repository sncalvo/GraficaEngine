#pragma once

#include <memory>

#include "./Mesh.h"
#include "./Shader.h"

#include "../Core/Transform.h"

namespace Engine {
    class ShadowRenderer
    {
    public:
        ShadowRenderer(std::shared_ptr<Mesh> mesh);
        std::shared_ptr<ShadowRenderer> clone()
        {
            return std::make_shared<ShadowRenderer>(_mesh);
        }
        void draw();
        void setTransform(Transform* transform) { _transform = transform; }
        static std::shared_ptr<Shader> getShader();

    private:
        static std::shared_ptr<Shader> _shader;
        std::shared_ptr<Mesh> _mesh;
        Transform* _transform;
    };
}
