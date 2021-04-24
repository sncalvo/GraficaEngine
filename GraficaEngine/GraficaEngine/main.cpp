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
#include "Core/GameLoop.h"
#include "Core/Camera.h"
#include "Core/GameObject.h"
#include "Core/Time.h"
#include "Core/Behaviour.h"

#include "Scripts/PlayerController.h"
#include "Scripts/CameraController.h"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGTH = 600;

int main(int argc, char* argv[])
{
	if (!Engine::MediaLayer::init())
	{
		return 1;
	}

	Engine::GameLoop gameLoop = Engine::GameLoop::getInstance();

	Engine::Window* window = new Engine::Window(WINDOW_WIDTH, WINDOW_HEIGTH, "Grafica Engine");
	gameLoop.addWindow(window);
	Engine::Scene* scene = new Engine::Scene();
	Engine::Shader* shader = new Engine::Shader();
	gameLoop.addShader(shader);

	const char* path = "Assets/Models/box.obj";
	Engine::Model cube(_strdup(path));
	Engine::MaterialObject material(shader);
	Engine::GameObject* box = new Engine::GameObject(cube, material);
	box->addBehaviour(new PlayerController());
	box->addBehaviour(new CameraController());

	scene->addGameObject(box);
	gameLoop.setActiveScene(scene);
	gameLoop.start();

	return 0;
}
