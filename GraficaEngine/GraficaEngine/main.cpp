#include <iostream>
#include <tuple>

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/GLU.h>

#include "MediaLayer.h"
#include "Camera.h"
#include "Window.h"
#include "Input.h"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGTH = 600;

int main(int argc, char* argv[])
{
	if (!MediaLayer::init())
	{
		return 1;
	}

	Window* window = new Window(WINDOW_WIDTH, WINDOW_HEIGTH, "Grafica Engine");

	Camera camera(window->getWidth(), window->getHeight());

	Input& input = Input::getInstance();

	while (true)
	{
		input.update();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (input.getKey(KEY_ESCAPE))
			break;

		window->swap();
	}

	delete window;
	MediaLayer::exit();
	
	return 0;
}
