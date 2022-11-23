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
#include "Renderer/Skybox.h"
#include "Core/GameLoop.h"
#include "Core/Light.h"
#include "Core/GameObject.h"
#include "Core/PerspectiveCamera.h"
#include "Core/OrthographicCamera.h"
#include "Core/Time.h"
#include "Core/Behaviour.h"
#include "Core/SceneManager.h"
#include "Physics/Collider.h"
#include "Core/Canvas.h"
#include "Core/Colors.h"

#include "Scripts/PlayerController.h"
#include "Scripts/JumpController.h"
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
#include "Scripts/CoinController.h"
#include "Scripts/StaticSpawner.h"
#include "Scripts/TimeController.h"
#include "Scripts/HudController.h"
#include "Scripts/HintController.h"
#include "Scripts/RiverMover.h"
#include "Scripts/TreeSpawner.h"
#include "Scripts/HomeScreenController.h"
#include "Scripts/MoveDownOnStart.h"
#include "Scripts/GameKeysHomeController.h"

#include "Core/Settings.h"

Engine::Scene *loadMainScene();
void loadHUD(Engine::Scene *);

int main(int argc, char *argv[])
{
	if (!Engine::MediaLayer::init())
	{
		return 1;
	}

	unsigned int width, height;
	std::tie(width, height) = Engine::Settings::getInstance().getWindowSize();

	Engine::GameLoop gameLoop = Engine::GameLoop::getInstance();

	Engine::Window *window = new Engine::Window(width, height, "Grafica Engine");
	gameLoop.addWindow(window);

	gameLoop.start();

	return 0;
}

Engine::Scene* loadMainScene()
{
	Engine::PerspectiveCamera *centeredFixedCamera = new Engine::PerspectiveCamera(
		glm::vec3(1.f, 4.f, 3.f),
		glm::vec3(0.f, 1.f, 0.f),
		Engine::YAW,
		-20.f);
	centeredFixedCamera->addBehaviour(new OffsetPlayer(glm::vec3(0.f, 0.f, 8.5f)));

	Engine::PerspectiveCamera* firstPersonCamera = new Engine::PerspectiveCamera(
		glm::vec3(1.f, 2.f, -0.5f));
	firstPersonCamera->addBehaviour(new FirstPersonCameraController(glm::vec3(.8f, 2.2f, 0.f)));

	Engine::PerspectiveCamera* thirdPersonCamera = new Engine::PerspectiveCamera(
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 1.f, 0.f),
		Engine::YAW,
		-30.f);
	thirdPersonCamera->addBehaviour(new ThirdPersonCameraController(10.f, glm::vec3(0.f, 1.f, 0.f)));

	Engine::PerspectiveCamera* flyingCamera = new Engine::PerspectiveCamera(
		glm::vec3(0.f, 5.f, 5.f),
		glm::vec3(0.f, 1.f, 0.f),
		Engine::YAW,
		-45.f);
	flyingCamera->addBehaviour(new FlyingCameraController());

	Engine::Scene* scene = new Engine::Scene(thirdPersonCamera);

	scene->addCamera("flying", flyingCamera);
	scene->addCamera("centeredFixed", centeredFixedCamera);
	scene->addCamera("firstPerson", firstPersonCamera);
	scene->addCamera("thirdPerson", thirdPersonCamera);

	Engine::BaseGameObject* cameraManager = new Engine::BaseGameObject();
	cameraManager->addBehaviour(new SwapCameras());
	scene->addGameObject(cameraManager);

	Engine::GameObject* duck = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/chicken.obj")),
		Engine::MaterialObject());
	duck->setCollider(new Engine::Collider(glm::vec3(-0.6f, 0.f, -0.6f), glm::vec3(0.6f, 2.5f, 0.6f)));
	duck->addBehaviour(new PlayerController());
	duck->addBehaviour(new JumpController());
	scene->addGameObject(duck);
	duck->addTag("player");
	duck->transform.position = glm::vec3(0.0f, 1.f, 0.f);
	duck->transform.lookAt(glm::vec3(0.0f, 0.f, -1.f));

	Engine::GameObject* grass = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/grass.obj")),
		Engine::MaterialObject());
	grass->addTag("ground");
	grass->setCollider(new Engine::Collider(glm::vec3(-24.f, 0.0f, -3.f), glm::vec3(24.f, 0.0f, 3.f)));

	for (int index = -5; index < 5; index++)
	{
		Engine::GameObject *newgrass = new Engine::GameObject(grass);
		newgrass->transform.scale = glm::vec3(1.f, 1.f, .5f);
		newgrass->transform.position = glm::vec3(0.f, 0.f, index * SPACE_BETWEEN_ROWS);
		scene->addGameObject(newgrass);
	}

	Engine::GameObject* river = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/river.obj")),
		Engine::MaterialObject());
	river->setCollider(new Engine::Collider(glm::vec3(-24.f, 0.f, -2.5f), glm::vec3(24.f, 0.f, 2.5f)));
	river->addBehaviour(new Hazard());
	river->addBehaviour(new RiverMover(-0.5f));

	Engine::GameObject* road = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/road.obj")),
		Engine::MaterialObject());
	road->setCollider(new Engine::Collider(glm::vec3(-24.f, 0.f, -3.f), glm::vec3(24.f, 0.f, 3.f)));
	road->addTag("ground");

	Engine::GameObject* log = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/log.obj")),
		Engine::MaterialObject());
	log->setCollider(new Engine::Collider(glm::vec3(-3.0, -.7f, -.5f), glm::vec3(3.0f, .7f, .5f)));
	log->addBehaviour(new Boundary(-30.f, 30.f));
	log->addTag("ground");

	river->addBehaviour(new ObstacleSpawner(Obstacles{ log }));

	Engine::GameObject* car = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/lowpolycar.obj")),
		Engine::MaterialObject());
	car->setCollider(new Engine::Collider(glm::vec3(-2.f, 0, -1.f), glm::vec3(2.f, 3.f, 1.f)));
	car->addBehaviour(new Hazard());
	car->addBehaviour(new Boundary(-30.f, 30.f));
	car->addTag("hazard");

	Engine::GameObject* truck = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/truck.obj")),
		Engine::MaterialObject());
	truck->setCollider(new Engine::Collider(glm::vec3(-2.5f, 0, -1.f), glm::vec3(2.5f, 3.5f, 1.f)));
	truck->addBehaviour(new Hazard());
	truck->addBehaviour(new Boundary(-30.f, 30.f));
	truck->addTag("hazard");

	Engine::GameObject* pickuptruck = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/pickuptruck.obj")),
		Engine::MaterialObject());
	pickuptruck->setCollider(new Engine::Collider(glm::vec3(-2.f, 0, -1.f), glm::vec3(2.f, 3.0f, 1.f)));
	pickuptruck->addBehaviour(new Hazard());
	pickuptruck->addBehaviour(new Boundary(-30.f, 30.f));
	pickuptruck->addTag("hazard");

	road->addBehaviour(new ObstacleSpawner(Obstacles{car, pickuptruck, truck}));

	Engine::BaseGameObject* spawner = new Engine::BaseGameObject();
	std::vector<Environment> environments{ grass, grass, river, road };

	spawner->addBehaviour(new EndlessSpawner(environments));
	scene->addGameObject(spawner);

	Engine::GameObject* tree = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/tree.obj")),
		Engine::MaterialObject());
	scene->addGameObject(tree);
	tree->transform.position = glm::vec3(-4.0f, 0.f, 5.f);

	Engine::GameObject* coin = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/coin.obj")),
		Engine::MaterialObject());
	coin->setCollider(new Engine::Collider(glm::vec3(-.5f, 0, -.5f), glm::vec3(.5f, 0.f, .5f)));
	coin->addBehaviour(new CoinController());
	coin->transform.rotateX(90.0f);
	coin->transform.scale = glm::vec3(.4f);
	coin->addTag("coin");

	Engine::GameObject* spanwableTree = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/tree.obj")),
		Engine::MaterialObject());
	spanwableTree->addTag("tree");

	grass->addBehaviour(new StaticSpawner(coin));
	grass->addBehaviour(new TreeSpawner(spanwableTree));

	road->addBehaviour(new StaticSpawner(coin));

	unsigned int width, height;
	std::tie(width, height) = Engine::Settings::getInstance().getWindowSize();

	Engine::Canvas* hint = new Engine::Canvas(glm::vec2(300, 80));
	hint->setColor(Engine::GRAY);
	hint->transform.position = glm::vec3((float)width, 0.f, 1.f);

	Engine::TextObject* hintText = new Engine::TextObject("Change camera with V");
	hintText->setFont("eight-bit-dragon_small.otf");
	hintText->transform.position = glm::vec3(20.f, 15.f, 2.f);
	hintText->setColor(Engine::BLACK);
	hint->addChild(hintText);

	scene->addGameObject(hint);
	hint->addBehaviour(new HintController(5.f, 300.f));

	std::vector<std::string> faces {
		"Assets/Skybox/right.jpg",
		"Assets/Skybox/left.jpg",
		"Assets/Skybox/bottom.jpg",
		"Assets/Skybox/top.jpg",
		"Assets/Skybox/front.jpg",
		"Assets/Skybox/back.jpg",
	};
	Engine::Skybox *skybox = new Engine::Skybox(faces);
	scene->setSkybox(skybox);

	Engine::Canvas* homeScreen = new Engine::Canvas(glm::vec2(376, 35));
	homeScreen->setColor(Engine::BLACK);
	homeScreen->transform.position = glm::vec3(230.f, 320.f, 1.f);
	Engine::TextObject* crossyRoad = new Engine::TextObject("Crossii Road");
	crossyRoad->setColor(Engine::WHITE);
	crossyRoad->setFont("8bit-wonder_large.TTF");
	crossyRoad->transform.position = glm::vec3(1.f);
	homeScreen->addChild(crossyRoad);
	homeScreen->addBehaviour(new HomeScreenController());
	scene->addGameObject(homeScreen);

	Engine::Canvas* gameKeysBack = new Engine::Canvas(glm::vec2(298, 19));
	gameKeysBack->setColor(Engine::BLACK);
	gameKeysBack->transform.position = glm::vec3(280.f, 280.f, 1.f);
	Engine::TextObject* gameKeys = new Engine::TextObject("Move with W A S D");
	gameKeys->setColor(Engine::WHITE);
	gameKeys->setFont("8bit-wonder.TTF");
	gameKeys->transform.position = glm::vec3(1.f);
	gameKeys->addTag("text");
	gameKeysBack->addChild(gameKeys);
	gameKeysBack->addBehaviour(new GameKeysHomeController());
	scene->addGameObject(gameKeysBack);

	return scene;
}

void loadHUD(Engine::Scene* scene)
{
	unsigned int width, height;
	std::tie(width, height) = Engine::Settings::getInstance().getWindowSize();

	Engine::TextObject* settingsTitle = new Engine::TextObject("Configurations");
	settingsTitle->transform.position = glm::vec3(100.0f, 25.0f, 1.0f);
	settingsTitle->setFont("8bit-wonder.TTF");
	settingsTitle->setColor(Engine::BLACK);

	Engine::TextObject* wireframeTitle = new Engine::TextObject("Wireframe(Y):");
	wireframeTitle->transform.position = glm::vec3(3.0f, 5.0f, 1.0f);
	wireframeTitle->setFont("eight-bit-dragon_small.otf");
	wireframeTitle->setColor(Engine::BLACK);

	Engine::TextObject* wireframeValue = new Engine::TextObject("OFF");
	wireframeValue->transform.position = glm::vec3(68.0f, 5.0f, 1.0f);
	wireframeValue->setFont("eight-bit-dragon_small.otf");
	wireframeValue->setColor(Engine::RED);
	wireframeValue->addTag("wireframe_value");

	Engine::TextObject* texturesTitle = new Engine::TextObject("Textures(T):");
	texturesTitle->setFont("eight-bit-dragon_small.otf");
	texturesTitle->transform.position = glm::vec3(185.0f, 5.0f, 2.0f);
	texturesTitle->setColor(Engine::BLACK);

	Engine::TextObject* textureValue = new Engine::TextObject("ON");
	textureValue->setFont("eight-bit-dragon_small.otf");
	textureValue->transform.position = glm::vec3(248.0f, 5.0f, 0.1f);
	textureValue->setColor(Engine::GREEN);
	textureValue->addTag("texture_value");

	Engine::TextObject* speedTitle = new Engine::TextObject("Speed(1, 2, 3):");
	speedTitle->transform.position = glm::vec3(3.0f, 15.0f, 1.0f);
	speedTitle->setFont("eight-bit-dragon_small.otf");
	speedTitle->setColor(Engine::BLACK);

	Engine::TextObject* speedValue = new Engine::TextObject("2");
	speedValue->transform.position = glm::vec3(85.0f, 15.0f, 1.0f);
	speedValue->setFont("eight-bit-dragon_small.otf");
	speedValue->setColor(Engine::BLACK);
	speedValue->addTag("speed_value");

	Engine::TextObject* interpolateTitle = new Engine::TextObject("Interpolated(U):");
	interpolateTitle->setFont("eight-bit-dragon_small.otf");
	interpolateTitle->transform.position = glm::vec3(90.0f, 5.0f, 1.0f);
	interpolateTitle->setColor(Engine::BLACK);

	Engine::TextObject* interpolateValue = new Engine::TextObject("OFF");
	interpolateValue->setFont("eight-bit-dragon_small.otf");
	interpolateValue->transform.position = glm::vec3(163.0f, 5.0f, 1.0f);
	interpolateValue->setColor(Engine::RED);
	interpolateValue->addTag("interpolation_value");

	Engine::TextObject* lightsTitle = new Engine::TextObject("Ambient Light(O):");
	lightsTitle->setFont("eight-bit-dragon_small.otf");
	lightsTitle->transform.position = glm::vec3(98.0f, 15.0f, 2.0f);
	lightsTitle->setColor(Engine::BLACK);

	Engine::TextObject* lightsValue = new Engine::TextObject("Noon");
	lightsValue->setFont("eight-bit-dragon_small.otf");
	lightsValue->transform.position = glm::vec3(178.0f, 15.0f, 0.1f);
	lightsValue->setColor(Engine::BLACK);
	lightsValue->addTag("light_value");

	Engine::Canvas *hudDivider = new Engine::Canvas(glm::vec2(2.0f, 70.0f));
	hudDivider->transform.position = glm::vec3(560.0f, (float)height, 4.0f);
	hudDivider->setColor(Engine::BLACK);
	hudDivider->addBehaviour(new MoveDownOnStart(67.f));

	Engine::TextObject *gameplayTitle = new Engine::TextObject("Game");
	gameplayTitle->setFont("8bit-wonder.TTF");
	gameplayTitle->transform.position = glm::vec3(310.0f, 25.0f, 1.0f);
	gameplayTitle->setColor(Engine::BLACK);

	Engine::TextObject *scoreTitle = new Engine::TextObject("Score:");
	scoreTitle->setFont("eight-bit-dragon_small.otf");
	scoreTitle->transform.position = glm::vec3(300.0f, 5.0f, 1.0f);
	scoreTitle->setColor(Engine::BLACK);

	Engine::TextObject *scoreValue = new Engine::TextObject("0");
	scoreValue->setFont("eight-bit-dragon_small.otf");
	scoreValue->transform.position = glm::vec3(340.0f, 5.0f, 1.0f);
	scoreValue->setColor(Engine::ORANGE);
	scoreValue->addTag("score_value");

	Engine::TextObject *timeTitle = new Engine::TextObject("Time:");
	timeTitle->setFont("eight-bit-dragon_small.otf");
	timeTitle->transform.position = glm::vec3(300.0f, 15.0f, 1.0f);
	timeTitle->setColor(Engine::BLACK);

	Engine::TextObject *timeValue = new Engine::TextObject("00:00");
	timeValue->setFont("eight-bit-dragon_small.otf");
	timeValue->transform.position = glm::vec3(340.0f, 15.0f, 1.0f);
	timeValue->setColor(Engine::ORANGE);
	timeValue->addTag("time_value");
	timeValue->addBehaviour(new TimeController());

	Engine::Canvas *hud = new Engine::Canvas(glm::vec2((float)width, 70.0f));
	hud->transform.position = glm::vec3(0.0f, (float)height, 3.0f);
	hud->setColor(Engine::GRAY);
	hud->addBehaviour(new HudController());
	hud->addBehaviour(new MoveDownOnStart(67.f));
	scene->addGameObject(hud);

	hud->addChild(settingsTitle);

	hud->addChild(wireframeTitle);
	hud->addChild(wireframeValue);
	hud->addChild(texturesTitle);
	hud->addChild(textureValue);
	hud->addChild(speedTitle);
	hud->addChild(speedValue);
	hud->addChild(interpolateTitle);
	hud->addChild(interpolateValue);
	hud->addChild(lightsTitle);
	hud->addChild(lightsValue);

	scene->addGameObject(hudDivider);

	hud->addChild(gameplayTitle);

	hud->addChild(scoreTitle);
	hud->addChild(scoreValue);
	hud->addChild(timeTitle);
	hud->addChild(timeValue);
}
