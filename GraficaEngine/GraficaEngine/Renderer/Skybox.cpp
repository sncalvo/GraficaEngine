#include "Skybox.h"

#include <iostream>

#include <GL/glew.h>
#include <SDL/SDL_opengl.h>

#include <Freeimage/FreeImage.h>

namespace Engine
{
	Skybox::Skybox(std::vector<std::string> faces)
	{
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height;
        void *data;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(faces[i].c_str());
            FIBITMAP* bitmap = FreeImage_Load(format, faces[i].c_str());
            bitmap = FreeImage_ConvertTo24Bits(bitmap);
            width = FreeImage_GetWidth(bitmap);
            height = FreeImage_GetHeight(bitmap);
            data = FreeImage_GetBits(bitmap);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data
                );
            }
            else
            {
                std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        _id = textureID;

        _shader = new Shader("Assets/Shaders/skybox.vs", "Assets/Shaders/skybox.fs");
	
        glGenVertexArrays(1, &_VAO);
        glGenBuffers(1, &_VBO);
        glBindVertexArray(_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        float* vertices = _getVertices();
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6 * 3, vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
        delete vertices;
    }

    float* Skybox::_getVertices() const
    {
        return new float[] {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };
    }

    void Skybox::draw(glm::mat4 projection, glm::mat4 view, Light &light)
    {
        glDepthMask(GL_FALSE);
        _shader->use();
        _shader->setMat4("projection", projection);
        glm::mat4 viewWithoutTranslation = glm::mat4(glm::mat3(view));
        _shader->setMat4("view", viewWithoutTranslation);
        light.apply(*_shader);
        glBindVertexArray(_VAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);
    }
}