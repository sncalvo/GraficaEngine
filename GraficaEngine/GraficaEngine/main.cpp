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
#include "Core/Light.h"
#include "Core/GameObject.h"
#include "Core/Time.h"
#include "Core/Behaviour.h"

#include "TestEmpty.h"
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

	Engine::Model* duckModel = new Engine::Model(_strdup("Assets/Models/duck.obj"));
	Engine::MaterialObject duckMaterial(shader);
	Engine::GameObject* duck = new Engine::GameObject(duckModel, duckMaterial);
	duck->addBehaviour(new PlayerController());
	duck->addBehaviour(new CameraController());
	scene->addGameObject(duck);
	duck->transform.position += glm::vec3(1.0f, 0.f, 0.f);
	duck->transform.scale = glm::vec3(.5f);

	Engine::Model* logModel = new Engine::Model(_strdup("Assets/Models/log.obj"));
	Engine::MaterialObject logMaterial(shader);
	Engine::GameObject* log = new Engine::GameObject(logModel, logMaterial);
	scene->addGameObject(log);
	log->transform.position += glm::vec3(4.0f, 0.f, -5.f);

	Engine::Model* carModel = new Engine::Model(_strdup("Assets/Models/lowpolycar.obj"));
	Engine::MaterialObject carMaterial(shader);
	Engine::GameObject* car = new Engine::GameObject(carModel, carMaterial);
	scene->addGameObject(car);
	car->transform.position += glm::vec3(-3.0f, 0.f, 3.f);

	Engine::Model* treeModel = new Engine::Model(_strdup("Assets/Models/tree.obj"));
	Engine::MaterialObject treeMaterial(shader);
	Engine::GameObject* tree = new Engine::GameObject(treeModel, treeMaterial);
	scene->addGameObject(tree);
	tree->transform.position += glm::vec3(4.0f, 0.f, 5.f);

	Engine::Light* light = new Engine::Light(glm::vec3(.4f), glm::vec3(1.f), glm::vec3(1.5f), glm::vec3(1.f, 1.f, 1.f));
	scene->addLight(light);

	gameLoop.setActiveScene(scene);
	gameLoop.start();

	return 0;
}
