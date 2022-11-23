#include "ResourceManager.h"

namespace Engine
{
    ResourceManager::ResourceManager()
    {};

    Texture* ResourceManager::createTexture(const char* path, std::string type)
    {
        std::string pathStr = path;

        Texture* texture = _textures[pathStr];
        if (texture == nullptr)
        {
            texture = new Texture(path, type);
            _textures[pathStr] = texture;
        }

        return texture;
    }
}
