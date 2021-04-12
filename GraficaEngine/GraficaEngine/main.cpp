#include <iostream>
#include <tuple>

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/GLU.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MediaLayer.h"
#include "Camera.h"
#include "Window.h"
#include "Input.h"
#include "Model.h"
#include "Time.h"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGTH = 600;

int main(int argc, char* argv[])
{
	if (!MediaLayer::init())
	{
		return 1;
	}

	Window* window = new Window(WINDOW_WIDTH, WINDOW_HEIGTH, "Grafica Engine");

	Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));

	Input& input = Input::getInstance();

	glewInit();
	glewExperimental = GL_TRUE;

	const char* path = "GroundProps.obj";
	Model cube(_strdup(path));

	Shader shader("testShader.vs", "testShader.fs");

	while (true)
	{
		Time::updateTime();
		input.update();
		int movementX, movementY;
		std::tie(movementX, movementY) = input.getMouseMovement();

		std::cout << float(movementX) * Time::getDeltaTime() << ", " << float(movementY) * Time::getDeltaTime() << std::endl;
		// camera.processMouseMovement(float(movementX) * Time::getDeltaTime(), float(movementY) * Time::getDeltaTime());

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (input.getKey(KEY_ESCAPE))
			break;

		shader.use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(.1f, .1f, .1f));
		shader.setMat4("model", model);

		cube.draw(shader);

		window->swap();
	}

	delete window;
	MediaLayer::exit();
	
	return 0;
}
