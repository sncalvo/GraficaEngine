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
#include "Core/AnimationBuilder.h"
#include "Core/Animator.h"

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
	srand((unsigned)time(0));

	if (!Engine::MediaLayer::init())
	{
		return 1;
	}

	unsigned int width, height;
	std::tie(width, height) = Engine::Settings::getInstance().getWindowSize();

	Engine::GameLoop gameLoop = Engine::GameLoop::getInstance();

	Engine::Window *window = new Engine::Window(width, height, "Grafica Engine");
	gameLoop.addWindow(window);

	Engine::Scene *scene = loadMainScene();
	loadHUD(scene);

	Engine::SceneManager &sceneManager = Engine::SceneManager::getInstance();
	sceneManager.addScene("main", scene);
	sceneManager.loadScene("main");

	gameLoop.start();

	return 0;
}

Engine::Scene* loadMainScene()
{
	Engine::AnimationBuilder animationBuilder;
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

	Engine::Scene* scene = new Engine::Scene(flyingCamera);

	scene->addCamera("flying", flyingCamera);
	scene->addCamera("centeredFixed", centeredFixedCamera);
	scene->addCamera("firstPerson", firstPersonCamera);
	scene->addCamera("thirdPerson", thirdPersonCamera);

	Engine::BaseGameObject* cameraManager = new Engine::BaseGameObject();
	cameraManager->addBehaviour(new SwapCameras());
	scene->addGameObject(cameraManager);

    Engine::Model* vampireModel = new Engine::Model(_strdup("Assets/Models/dancing_vampire.dae"));
	animationBuilder = Engine::AnimationBuilder(_strdup("Assets/Models/dancing_vampire.dae"),
		vampireModel);
	Engine::Animation* danceAnimation = animationBuilder.getAnimation();
	Engine::GameObject* vampire = new Engine::GameObject(
		vampireModel,
		Engine::MaterialObject(),
		danceAnimation);
	vampire->setCollider(new Engine::Collider(glm::vec3(-0.6f, 0.f, -0.6f), glm::vec3(0.6f, 2.5f, 0.6f)));
	vampire->addBehaviour(new PlayerController());
	vampire->addBehaviour(new JumpController());
	vampire->addTag("player");
	scene->addGameObject(vampire);

    Engine::Model* vampireModel2 = new Engine::Model(_strdup("Assets/Models/dancing_vampire.dae"));
	Engine::GameObject* vampire2 = new Engine::GameObject(
		vampireModel2,
		Engine::MaterialObject(),
		danceAnimation);
	vampire2->transform.scale = glm::vec3(3.5f, 3.5f, 3.5f);
	scene->addGameObject(vampire2);

	Engine::GameObject* level = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/City.obj")),
		Engine::MaterialObject());
	level->setCollider(new Engine::Collider(glm::vec3(-100.f, 0.0f, -100.f), glm::vec3(100.f, 0.0f, 100.f)));
	scene->addGameObject(level);

	Engine::GameObject* buildings = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/Building.obj")),
		Engine::MaterialObject());
	scene->addGameObject(buildings);

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

/* 	scene->addGameObject(hint); */
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
/* 	scene->addGameObject(homeScreen); */

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
/* 	scene->addGameObject(gameKeysBack); */

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
