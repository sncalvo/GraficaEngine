#pragma once

#include <glm/glm.hpp>

#include "../Core/FontManager.h"

namespace Engine {
    class TextRenderer
    {
    private:
        Engine::Font _font;
        glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f);
    public:
        TextRenderer()
        {
            _font = FontManager::getInstance()->getFont("arial_rounded.TTF"); //TODO add optional parameter with default font
        }
    };
}
