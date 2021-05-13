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
#include "Core/Light.h"
#include "Core/GameObject.h"
#include "Core/PerspectiveCamera.h"
#include "Core/OrthographicCamera.h"
#include "Core/Time.h"
#include "Core/Behaviour.h"
#include "Physics/Collider.h"

#include "Scripts/PlayerController.h"
#include "Scripts/SwapCameras.h"
#include "Scripts/FlyingCameraController.h"
#include "Scripts/OffsetPlayer.h"
#include "Scripts/FirstPersonCameraController.h"
#include "Scripts/ThirdPersonCameraController.h"
#include "Scripts/Mover.h"
#include "Scripts/Hazard.h"
#include "Scripts/Boundary.h"
#include "Scripts/EndlessSpawner.h"
#include "Scripts/ObstacleSpawner.h"
#include "Scripts/TextController.h"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGTH = 600;

int main(int argc, char *argv[])
{
	if (!Engine::MediaLayer::init())
	{
		return 1;
	}

	Engine::GameLoop gameLoop = Engine::GameLoop::getInstance();

	Engine::Window *window = new Engine::Window(WINDOW_WIDTH, WINDOW_HEIGTH, "Grafica Engine");
	gameLoop.addWindow(window);

	Engine::OrthographicCamera *isometricCamera = new Engine::OrthographicCamera(
		glm::vec3(0.f, 4.f, 0.f),
		glm::vec3(0.f, 1.f, 0.f),
		-35.264f - 90.f,
		-45.f);
	isometricCamera->addBehaviour(new OffsetPlayer(glm::vec3(0.f, 4.f, 0.f)));
	Engine::PerspectiveCamera *centeredFixedCamera = new Engine::PerspectiveCamera(
		glm::vec3(1.f, 4.f, 3.f),
		glm::vec3(0.f, 1.f, 0.f),
		Engine::YAW,
		-30.f);
	centeredFixedCamera->addBehaviour(new OffsetPlayer(glm::vec3(0.f, 4.5f, 5.f)));

	Engine::PerspectiveCamera *firstPersonCamera = new Engine::PerspectiveCamera(
		glm::vec3(1.f, 2.f, -0.5f));
	firstPersonCamera->addBehaviour(new FirstPersonCameraController(glm::vec3(0.5f, 1.f, 0.f)));

	Engine::PerspectiveCamera *thirdPersonCamera = new Engine::PerspectiveCamera(
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 1.f, 0.f),
		Engine::YAW,
		-30.f);
	thirdPersonCamera->addBehaviour(new ThirdPersonCameraController(10.f, glm::vec3(0.f, 1.f, 0.f)));

	Engine::PerspectiveCamera *flyingCamera = new Engine::PerspectiveCamera(
		glm::vec3(0.f, 5.f, 5.f),
		glm::vec3(0.f, 1.f, 0.f),
		Engine::YAW,
		-45.f);
	flyingCamera->addBehaviour(new FlyingCameraController());

	Engine::Scene *scene = new Engine::Scene(flyingCamera);
	scene->addCamera("isometric", isometricCamera);
	scene->addCamera("centeredFixed", centeredFixedCamera);
	scene->addCamera("firstPerson", firstPersonCamera);
	scene->addCamera("thirdPerson", thirdPersonCamera);

	Engine::Shader *shader = new Engine::Shader();
	gameLoop.addShader(shader);

	Engine::BaseGameObject *cameraManager = new Engine::BaseGameObject();
	cameraManager->addBehaviour(new SwapCameras());
	scene->addGameObject(cameraManager);

	Engine::GameObject *duck = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/duck.obj")),
		Engine::MaterialObject(shader)
	);
	duck->setCollider(new Engine::Collider(glm::vec3(-1.f), glm::vec3(1.f)));
	duck->addBehaviour(new PlayerController());
	scene->addGameObject(duck);
	duck->addTag("player");
	duck->transform.position += glm::vec3(1.0f, 0.f, 0.f);
	duck->transform.scale = glm::vec3(.5f);

	Engine::GameObject *floor = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/floor.obj")),
		Engine::MaterialObject(shader)
	);

	for (int index = -5; index < 5; index++)
	{
		Engine::GameObject *newFloor = new Engine::GameObject(floor);
		newFloor->transform.scale = glm::vec3(.5f);
		newFloor->transform.position = glm::vec3(0.f, 0.f, index * SPACE_BETWEEN_ROWS);
		scene->addGameObject(newFloor);
	}

	Engine::GameObject *river = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/river.obj")),
		Engine::MaterialObject(shader)
	);
	river->setCollider(new Engine::Collider(glm::vec3(-12.f, 0.f, -1.5f), glm::vec3(12.f, 0.f, 1.5f)));
	river->addBehaviour(new Hazard());

	Engine::GameObject *log = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/log.obj")),
		Engine::MaterialObject(shader)
	);
	log->setCollider(new Engine::Collider(glm::vec3(-1.f), glm::vec3(1.f)));
	log->addBehaviour(new Boundary(-30.f, 10.f));

	river->addBehaviour(new ObstacleSpawner(Obstacles { log }));

	Engine::GameObject *car = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/lowpolycar.obj")),
		Engine::MaterialObject(shader)
	);
	car->setCollider(new Engine::Collider(glm::vec3(-1.5f, 0, -1.f), glm::vec3(1.5f, 0.f, 1.f)));
	car->addBehaviour(new Hazard());
	car->addBehaviour(new Boundary(-30.f, 10.f));
	car->addTag("hazard");

	floor->addBehaviour(new ObstacleSpawner(Obstacles{ car }));

	Engine::BaseGameObject *spawner = new Engine::BaseGameObject();
	std::vector<Environment> environments { floor, river };
	spawner->addBehaviour(new EndlessSpawner(environments));
	scene->addGameObject(spawner);

	Engine::GameObject *tree = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/tree.obj")),
		Engine::MaterialObject(shader)
	);
	scene->addGameObject(tree);
	tree->transform.position += glm::vec3(4.0f, 0.f, 5.f);

	Engine::Light *light = new Engine::Light(glm::vec3(.4f), glm::vec3(1.f), glm::vec3(1.5f), glm::vec3(1.f, -1.f, -1.f));
	scene->addLight(light);

	Engine::TextObject* text = new Engine::TextObject("Quieto");
	text->transform.position += glm::vec3(25.0f, 25.0f, 0.0f);
	text->addBehaviour(new TextController());
	scene->addGameObject(text);

	gameLoop.setActiveScene(scene);
	gameLoop.start();

	return 0;
}
