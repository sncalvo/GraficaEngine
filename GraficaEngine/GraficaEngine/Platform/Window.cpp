#include <GL/glew.h>

#include "Window.h"
#include "../Utils/DebugLog.h"

void GLAPIENTRY MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	if (type == GL_DEBUG_TYPE_ERROR)
	{
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
	}
}

namespace Engine
{
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

		SDL_SetRelativeMouseMode(SDL_TRUE);
		SDL_ShowCursor(SDL_DISABLE);
		glewInit();

		// During init, enable debug output
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);
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
}
