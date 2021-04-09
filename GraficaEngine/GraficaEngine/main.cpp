#include <iostream>
#include <tuple>

#include "SDL.h"
#include "SDL_opengl.h"

#include <GL/GLU.h>

#include "MediaLayer.h"
#include "Camera.h"
#include "Window.h"
#include "Input.h"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGTH = 600;

int main(int argc, char* argv[])
{
	MediaLayer::init();
	Window window(WINDOW_WIDTH, WINDOW_HEIGTH, "Grafica Engine");

	Camera camera(window.getWith(), window.getHeight());
	Input& input = Input::getInstance();

	while (true)
	{
		input.update(0.f, 0.f);

		if (input.getKey(KEY_ESCAPE))
			break;

		window.swap();
	}

	window.destroy();
	MediaLayer::exit();
	return 0;
}
