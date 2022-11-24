#pragma once

#include <string>
#include <map>

#include "Texture.h"

namespace Engine {
	class ResourceManager
	{
	public:
		static ResourceManager& getInstance()
		{
			static ResourceManager _instance;
			return _instance;
		}

		Texture* createTexture(const char* path, std::string type);

	private:
		ResourceManager();

		std::map<std::string, Texture*> _textures{};
	};
}
