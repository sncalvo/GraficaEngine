#pragma once
#include <map>
#include <iostream>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gl/glew.h>

#include "BaseGameObject.h"
#include "../Renderer/Shader.h"
#include "Camera.h"
#include "FontManager.h"

namespace Engine
{

    constexpr auto DEFAULT_FONT_PATH = "Assets/Fonts/showg.ttf";

    class TextObject : public BaseGameObject
	{
	private:
		Engine::Font _font;
        std::string _text;
        glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f);
	public:
        TextObject(std::string text = "");
        void setText(std::string text);
        void draw() const;
        bool isDrawable() const;
		void setFont(std::string font);
	};
}
