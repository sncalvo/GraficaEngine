#include "Window.h"

#include "DebugLog.h"

Window::Window(int width, int height, std::string title) :
	_width(width),
	_height(height),
	_title(title)
{
	_window = SDL_CreateWindow(title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_OPENGL); // SDL_WINDOW_SHOWN?
	_context = SDL_GL_CreateContext(_window);
}

void Window::swap()
{
	SDL_GL_SwapWindow(_window);
}

void Window::setTitle(std::string title)
{
	if (_window)
		SDL_SetWindowTitle(_window, title.c_str());
}

int Window::getWidth()
{
	return _width;
}

int Window::getHeight()
{
	return _height;
}

void Window::destroy()
{
	if (_context)
	{
		SDL_GL_DeleteContext(_context);
		_context = nullptr;
	}

	if (_window)
	{
		SDL_DestroyWindow(_window);
		_window = nullptr;
	}
}

Window::~Window()
{
	destroy();
}
