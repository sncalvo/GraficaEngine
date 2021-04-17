#include <iostream>
#include <tuple>

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/GLU.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/MediaLayer.h"
#include "Platform/Window.h"
#include "Renderer/Model.h"
#include "Core/Camera.h"
#include "Core/GameObject.h"
#include "Core/Input.h"
#include "Core/Time.h"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGTH = 600;

int main(int argc, char* argv[])
{
	if (!Engine::MediaLayer::init())
	{
		return 1;
	}

	Engine::Window* window = new Engine::Window(WINDOW_WIDTH, WINDOW_HEIGTH, "Grafica Engine");
	Engine::Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));
	Engine::Input& input = Engine::Input::getInstance();

	Engine::Shader* shader = new Engine::Shader("Assets/Shaders/default.vs", "Assets/Shaders/default.fs");

	const char* path = "Assets/Models/box.obj";
	Engine::Model cube(_strdup(path));
	Engine::MaterialObject material(shader);
	Engine::GameObject* box = new Engine::GameObject(cube, material);

	path = "Assets/Models/box3.obj";
	Engine::Model cube2(_strdup(path));

	float speed = 10.f;

	glEnable(GL_DEPTH_TEST);

	while (true)
	{
		Engine::Time::updateTime();
		input.update();

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int movementX, movementY;
		std::tie(movementX, movementY) = input.getMouseMovement();

		camera.processMouseMovement(float(movementX) * Engine::Time::getDeltaTime(), float(movementY) * Engine::Time::getDeltaTime());
		if (input.getKey(Engine::KEY_W))
		{
			camera.processKeyboard(Engine::Camera_Movement::FORWARD, Engine::Time::getDeltaTime());
		}
		if (input.getKey(Engine::KEY_S))
		{
			camera.processKeyboard(Engine::Camera_Movement::BACKWARD, Engine::Time::getDeltaTime());
		}
		if (input.getKey(Engine::KEY_A))
		{
			camera.processKeyboard(Engine::Camera_Movement::LEFT, Engine::Time::getDeltaTime());
		}
		if (input.getKey(Engine::KEY_D))
		{
			camera.processKeyboard(Engine::Camera_Movement::RIGHT, Engine::Time::getDeltaTime());
		}

		if (input.getKey(Engine::KEY_ESCAPE))
			break;

		shader->use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		shader->setMat4("projection", projection);
		shader->setMat4("view", view);

		if (input.getKey(Engine::KEY_UP))
		{
			box->transform.position += glm::vec3(0.f, 0.f, 1.f) * speed * Engine::Time::getDeltaTime();
		}
		else if (input.getKey(Engine::KEY_DOWN))
		{
			box->transform.position -= glm::vec3(0.f, 0.f, 1.f) * speed * Engine::Time::getDeltaTime();
		}
		else if (input.getKey(Engine::KEY_LEFT))
		{
			box->transform.position -= glm::vec3(1.f, 0.f, 0.f) * speed * Engine::Time::getDeltaTime();
		}
		else if (input.getKey(Engine::KEY_RIGHT))
		{
			box->transform.position += glm::vec3(1.f, 0.f, 0.f) * speed * Engine::Time::getDeltaTime();
		}

		box->draw();

		window->swap();
	}

	delete window;
	Engine::MediaLayer::exit();
	
	return 0;
}
