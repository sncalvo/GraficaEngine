#pragma once

#include <map>
#include <string>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Renderer/Shader.h"

namespace Engine
{
	constexpr auto DEFAULT_FONTS_PATH = "Assets/Fonts/";

	struct Character {
		unsigned int textureID = 0;
		glm::ivec2 Size;
		glm::ivec2 Bearing;
		unsigned int Advance;
		unsigned int EBO;
	};

	struct Font {
		FT_Face face;
		std::map<char, Character> characters;
		unsigned int VAO;
	};

	class FontManager
	{
	private:
		std::map<std::string, Font> _fonts;
		Shader* _fontsShader;
		void setupFontsShader();
		FontManager();
	public:
		static FontManager* getInstance()
		{
			static FontManager* _instance = new FontManager();
			return _instance;
		}
		Font getFont(std::string fontName) const;
		Shader* getFontsShader() const;
		~FontManager();
	};
}
