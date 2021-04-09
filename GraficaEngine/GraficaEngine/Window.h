#pragma once

#include <string>

#include "SDL.h"

#include "Math.h"

class Window
{
private:
	std::string _title;
	int _width;
	int _height;
	SDL_Window* _window;
	SDL_GLContext _context;

public:
	Window(int width, int height, std::string title = "");
	void swap();
	void setTitle(std::string title);
	void destroy();
	int getWith();
	int getHeight();
};
