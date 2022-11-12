#pragma once

#include <memory>

#include "Mesh.h"

namespace Engine {
    class ShadowRenderer
    {
    public:
        ShadowRenderer(std::shared_ptr<Mesh> mesh);
        std::shared_ptr<ShadowRenderer> clone()
        {
            return std::make_shared<ShadowRenderer>(_mesh);
        }
    private:
        std::shared_ptr<Mesh> _mesh;
    };
}
