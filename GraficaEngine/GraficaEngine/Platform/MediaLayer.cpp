#include "MediaLayer.h"
#include "../Utils/DebugLog.h"

namespace Engine
{
	bool MediaLayer::init()
	{
		Uint32 flags = (SDL_INIT_EVENTS);

		if (SDL_Init(flags) < 0)
		{
			DebugLog::error("SDL_Init: Couldn't start SDL");
			DebugLog::error(std::string(SDL_GetError()));
			throw "SDL Init Fail";
		}

		//Use OpenGL 3.1 core
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		return true;
	}

	void MediaLayer::exit()
	{
		SDL_Quit();
	}
}
