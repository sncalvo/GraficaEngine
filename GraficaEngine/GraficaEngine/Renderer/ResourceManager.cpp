#include "ResourceManager.h"

#include <Freeimage/FreeImage.h>

#include "../Utils/DebugLog.h"

/**
FreeImage error handler
@param fif Format / Plugin responsible for the error
@param message Error message
*/
void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char* message) {
    if (fif != FIF_UNKNOWN) {
        printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
    }
    Engine::DebugLog::error("ERROR::Failed to load image: " + std::string(message));
}
// In your main program …

namespace Engine
{
    ResourceManager::ResourceManager()
    {
        FreeImage_SetOutputMessage(FreeImageErrorHandler);
    };

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
