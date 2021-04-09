#include "MediaLayer.h"
#include "DebugLog.h"

bool MediaLayer::init()
{
	if (SDL_Init(0) < 0)
	{
		DebugLog::error("SDL_Init: Couldn't start SDL");
		DebugLog::error(std::string(SDL_GetError()));
		throw "SDL Init Fail";
	}

	Uint32 flags = (SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	if (SDL_WasInit(flags) != 0)
	{
		DebugLog::error("SDL_WasInit: Tried to reinitailize Video");
		DebugLog::error(std::string(SDL_GetError()));
		throw "SDL_WasInit Fail";
	}
	if (SDL_InitSubSystem(flags) < 0)
	{
		DebugLog::error("SDL_Init: Couldn't start Video");
		DebugLog::error(std::string(SDL_GetError()));
		throw "SDL_Init Fail";
	}

	return true;
}

void MediaLayer::exit()
{
	SDL_Quit();
}
