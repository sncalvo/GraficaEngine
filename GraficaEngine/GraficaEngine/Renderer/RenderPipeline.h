#pragma once

#include "../Core/Scene.h"
#include "./Shader.h"

namespace Engine {

    class RenderPipeline
    {
    private:
        Shader* _defaultShader;
        Shader* _shadowShader;
        Shader* _hdrShader;
        Shader* _shaderBlur;

        void _drawShadows(Scene* scene);
        void _drawMeshes(Scene* scene);

        void _drawFrame();
    public:
        RenderPipeline();
        void setup();
        void draw(Scene* scene);
    };

}
