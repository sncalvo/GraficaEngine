#pragma once

#include "../Core/Scene.h"
#include "./Shader.h"

namespace Engine {

    class Renderer
    {
    private:
        Shader* _defaultShader;
        Shader* _shadowShader;

    public:
        Renderer();
        void setup();
        void draw(Scene* scene);
    };

}
