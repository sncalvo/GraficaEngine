#pragma once

#include "../Core/Scene.h"
#include "./Shader.h"
#include "../Core/Animator.h"

namespace Engine {

    class RenderPipeline
    {
    private:
        Shader* _defaultShader;
        Shader* _shadowShader;
        Shader* _hdrShader;
        Shader* _shaderBlur;

        void _drawShadows(Scene* scene);
<<<<<<< Updated upstream
        void _drawMeshes(Scene* scene);

        void _drawFrame();
=======
        void _drawMeshes(Scene* scene, Animator *a);
>>>>>>> Stashed changes
    public:
        RenderPipeline();
        void setup();
        void draw(Scene* scene, Animator *a);
    };

}
