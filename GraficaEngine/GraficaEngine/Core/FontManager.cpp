#include "FontManager.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <iostream>
#include <experimental/filesystem>
#include <gl/glew.h>

constexpr auto SMALL_SIZE = 15;
constexpr auto NORMAL_SIZE = 18;
constexpr auto LARGE_SIZE = 36;

namespace stdfs = std::experimental::filesystem;

namespace Engine
{
    std::vector<stdfs::path> get_filenames(stdfs::path path)
    {
        std::vector<stdfs::path> filenames;

        const stdfs::directory_iterator end;

        for (stdfs::directory_iterator iter(path); iter != end; ++iter)
        {
            if (stdfs::is_regular_file(*iter))
                filenames.push_back(iter->path());
        }

        return filenames;
    }

    void setFaceSize(FT_Face face, int size)
    {
        FT_Set_Pixel_Sizes(face, 0, size);
    }

    bool endsWith(std::string const& fullString, std::string const& ending);

	FontManager::FontManager()
	{
        setupFontsShader();

        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            return;
        }

        for (const auto& fontPath : get_filenames(DEFAULT_FONTS_PATH))
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
            Font newFont;
            if (FT_New_Face(ft, fontPath.string().c_str(), 0, &newFont.face))
            {
                std::cout << "ERROR::FREETYPE: Failed to load font " << fontPath << std::endl;
                continue;
            }

            int size = NORMAL_SIZE;
            if (endsWith(fontPath.filename().string(), "_large.TTF") || endsWith(fontPath.filename().string(), "_large.otf"))
            {
                size = LARGE_SIZE;
            }
            else if (endsWith(fontPath.filename().string(), "_small.TTF") || endsWith(fontPath.filename().string(), "_small.otf"))
            {
                size = SMALL_SIZE;
            }
            setFaceSize(newFont.face, size);

            glm::vec4* vertices = new glm::vec4[255 * 4];

            unsigned int index = 0;
            for (unsigned char c = 0; c < 255; c++)
            {
                // load character glyph 
                if (FT_Load_Char(newFont.face, c, FT_LOAD_RENDER))
                {
                    std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                    continue;
                }

                // generate texture
                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    newFont.face->glyph->bitmap.width,
                    newFont.face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    newFont.face->glyph->bitmap.buffer
                );
                // set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                unsigned int indices[] = {
                    index, index + 1, index + 2,
                        index, index + 2, index + 3
                };

                unsigned int EBO;
                glGenBuffers(1, &EBO);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

                // The EBO should be binded each time we want to draw a single character
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

                Character character {
                    texture,
                    glm::ivec2(newFont.face->glyph->bitmap.width, newFont.face->glyph->bitmap.rows),
                    glm::ivec2(newFont.face->glyph->bitmap_left, newFont.face->glyph->bitmap_top),
                    static_cast<unsigned int>(newFont.face->glyph->advance.x),
                    EBO
                };

                newFont.characters.insert(std::pair<char, Character>(c, character));

                float xpos = character.Bearing.x;
                float ypos = -(character.Size.y - character.Bearing.y);
                float w = character.Size.x;
                float h = character.Size.y;

                vertices[index].x = xpos;
                vertices[index].y = ypos + h;
                vertices[index].z = 0.0f;
                vertices[index].w = 0.0f;

                vertices[index + 1].x = xpos;
                vertices[index + 1].y = ypos;
                vertices[index + 1].z = 0.0f;
                vertices[index + 1].w = 1.0f;

                vertices[index + 2].x = xpos + w;
                vertices[index + 2].y = ypos;
                vertices[index + 2].z = 1.0f;
                vertices[index + 2].w = 1.0f;

                vertices[index + 3].x = xpos + w;
                vertices[index + 3].y = ypos + h;
                vertices[index + 3].z = 1.0f;
                vertices[index + 3].w = 0.0f;

                index += 4;
            }

            FT_Done_Face(newFont.face);

            glGenVertexArrays(1, &newFont.VAO);
            unsigned int VBO;
            glGenBuffers(1, &VBO);
            glBindVertexArray(newFont.VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 255 * 4, vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glBindVertexArray(0);

            delete[] vertices;

            // Save font
            _fonts[fontPath.filename().string()] = newFont;
        }
        FT_Done_FreeType(ft);
	}

    void FontManager::setupFontsShader()
    {
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        _fontsShader = new Shader("Assets/Shaders/font_shader.vs", "Assets/Shaders/font_shader.fs");

        // 2D projection
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(600), -5.0f, 5.0f); //TODO Configure with viewport's size
        _fontsShader->use();
        _fontsShader->setMatrix4f("projection", glm::value_ptr(projection));
    }

    Font FontManager::getFont(std::string fontName) const
    {
        std::map<std::string, Font>::const_iterator it = _fonts.find(fontName);
        if (it == _fonts.end())
        {
            throw new std::exception("Font not found.");
        }
        else
        {
            return it->second;
        }
    }

    Shader* FontManager::getFontsShader() const
    {
        return _fontsShader;
    }

    bool endsWith(std::string const& fullString, std::string const& ending)
    {
        if (fullString.length() >= ending.length())
        {
            return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
        }
        else
        {
            return false;
        }
    }

    FontManager::~FontManager()
    {
        delete _fontsShader;
    }
}
