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

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGTH = 600;

Engine::Scene *loadMainScene(Engine::Shader *);
void loadHUD(Engine::Scene *);

int main(int argc, char *argv[])
{
	if (!Engine::MediaLayer::init())
	{
		return 1;
	}

	Engine::GameLoop gameLoop = Engine::GameLoop::getInstance();

	Engine::Window *window = new Engine::Window(WINDOW_WIDTH, WINDOW_HEIGTH, "Grafica Engine");
	gameLoop.addWindow(window);

	Engine::Shader *shader = new Engine::Shader();
	gameLoop.addShader(shader);

	Engine::Scene *scene = loadMainScene(shader);
	loadHUD(scene);

	Engine::SceneManager &sceneManager = Engine::SceneManager::getInstance();
	sceneManager.addScene("main", scene);
	sceneManager.loadScene("main");

	gameLoop.start();

	return 0;
}

Engine::Scene *loadMainScene(Engine::Shader *shader)
{
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
		-20.f);
	centeredFixedCamera->addBehaviour(new OffsetPlayer(glm::vec3(0.f, 0.f, 8.5f)));

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

	Engine::BaseGameObject *cameraManager = new Engine::BaseGameObject();
	cameraManager->addBehaviour(new SwapCameras());
	scene->addGameObject(cameraManager);

	Engine::GameObject *duck = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/chicken.obj")),
		Engine::MaterialObject(shader));
	duck->setCollider(new Engine::Collider(glm::vec3(-0.6f, 0.f, -0.6f), glm::vec3(0.6f, 2.5f, 0.6f)));
	duck->addBehaviour(new PlayerController());
	duck->addBehaviour(new JumpController());
	scene->addGameObject(duck);
	duck->addTag("player");
	duck->transform.position = glm::vec3(1.0f, 1.f, 0.f);

	Engine::GameObject *grass = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/grass.obj")),
		Engine::MaterialObject(shader));
	grass->addTag("ground");
	grass->setCollider(new Engine::Collider(glm::vec3(-24.f, 0.0f, -3.f), glm::vec3(24.f, 0.0f, 3.f)));

	for (int index = -5; index < 5; index++)
	{
		Engine::GameObject *newgrass = new Engine::GameObject(grass);
		newgrass->transform.scale = glm::vec3(.5f);
		newgrass->transform.position = glm::vec3(0.f, 0.f, index * SPACE_BETWEEN_ROWS);
		scene->addGameObject(newgrass);
	}

	Engine::GameObject *river = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/river.obj")),
		Engine::MaterialObject(shader));
	river->setCollider(new Engine::Collider(glm::vec3(-24.f, 0.f, -3.f), glm::vec3(24.f, 0.f, 3.f)));
	river->addBehaviour(new Hazard());
	river->addBehaviour(new RiverMover(-0.5f));

	Engine::GameObject *road = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/road.obj")),
		Engine::MaterialObject(shader));
	road->setCollider(new Engine::Collider(glm::vec3(-24.f, 0.f, -3.f), glm::vec3(24.f, 0.f, 3.f)));
	road->addTag("ground");

	Engine::GameObject *log = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/log.obj")),
		Engine::MaterialObject(shader));
	log->setCollider(new Engine::Collider(glm::vec3(-2.5, -.7f, -.5f), glm::vec3(2.5f, .7f, .5f)));
	log->addBehaviour(new Boundary(-30.f, 30.f));
	log->addTag("ground");

	river->addBehaviour(new ObstacleSpawner(Obstacles{log}));

	Engine::GameObject *car = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/lowpolycar.obj")),
		Engine::MaterialObject(shader));
	car->setCollider(new Engine::Collider(glm::vec3(-1.5f, 0, -1.f), glm::vec3(1.5f, 1.5f, 1.f)));
	car->addBehaviour(new Hazard());
	car->addBehaviour(new Boundary(-30.f, 30.f));
	car->addTag("hazard");

	road->addBehaviour(new ObstacleSpawner(Obstacles{car}));

	Engine::BaseGameObject *spawner = new Engine::BaseGameObject();
	std::vector<Environment> environments{grass, grass, river, road};

	spawner->addBehaviour(new EndlessSpawner(environments));
	scene->addGameObject(spawner);

	Engine::GameObject *tree = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/tree.obj")),
		Engine::MaterialObject(shader));
	scene->addGameObject(tree);
	tree->transform.position += glm::vec3(4.0f, 0.f, 5.f);

	Engine::GameObject *coin = new Engine::GameObject(
		new Engine::Model(_strdup("Assets/Models/coin.obj")),
		Engine::MaterialObject(shader));
	coin->setCollider(new Engine::Collider(glm::vec3(-.5f, 0, -.5f), glm::vec3(.5f, 0.f, .5f)));
	coin->addBehaviour(new CoinController());
	coin->transform.rotateX(90.0f);
	coin->transform.scale = glm::vec3(.4f);
	coin->addTag("coin");

	grass->addBehaviour(new StaticSpawner(coin));
	road->addBehaviour(new StaticSpawner(coin));

	Engine::Canvas *hint = new Engine::Canvas(glm::vec2(300, 80));
	hint->setColor(Engine::GRAY);
	hint->transform.position = glm::vec3(800.f, 0.f, 1.f);

	Engine::TextObject *hintText = new Engine::TextObject("Cambiá de cámara con V");
	hintText->transform.position = glm::vec3(15.f, 15.f, 2.f);
	hintText->setColor(Engine::BLACK);
	hint->addChild(hintText);

	scene->addGameObject(hint);
	hint->addBehaviour(new HintController(5.f, 300.f));

	return scene;
}

void loadHUD(Engine::Scene *scene)
{
	Engine::TextObject *settingsTitle = new Engine::TextObject("Game Settings");
	settingsTitle->transform.position = glm::vec3(100.0f, 25.0f, 1.0f);
	settingsTitle->setColor(Engine::BLACK);

	Engine::TextObject *wireframeTitle = new Engine::TextObject("Wireframe(Y):");
	wireframeTitle->transform.position = glm::vec3(10.0f, 5.0f, 1.0f);
	wireframeTitle->setColor(Engine::BLACK);

	Engine::TextObject *wireframeValue = new Engine::TextObject("OFF");
	wireframeValue->transform.position = glm::vec3(73.0f, 5.0f, 1.0f);
	wireframeValue->setColor(Engine::RED);
	wireframeValue->addTag("wireframe_value");

	Engine::TextObject *texturesTitle = new Engine::TextObject("Textures(T):");
	texturesTitle->transform.position = glm::vec3(90.0f, 15.0f, 2.0f);
	texturesTitle->setColor(Engine::BLACK);

	Engine::TextObject *textureValue = new Engine::TextObject("ON");
	textureValue->transform.position = glm::vec3(145.0f, 15.0f, 0.1f);
	textureValue->setColor(Engine::GREEN);
	textureValue->addTag("texture_value");

	Engine::TextObject *speedTitle = new Engine::TextObject("Speed(1, 2, 3):");
	speedTitle->transform.position = glm::vec3(10.0f, 15.0f, 1.0f);
	speedTitle->setColor(Engine::BLACK);

	Engine::TextObject *speedValue = new Engine::TextObject("1");
	speedValue->transform.position = glm::vec3(77.0f, 15.0f, 1.0f);
	speedValue->setColor(Engine::BLACK);
	speedValue->addTag("speed_value");

	Engine::TextObject *interpolateTitle = new Engine::TextObject("Interpolate(U):");
	interpolateTitle->transform.position = glm::vec3(100.0f, 5.0f, 1.0f);
	interpolateTitle->setColor(Engine::BLACK);

	Engine::TextObject *interpolateValue = new Engine::TextObject("OFF");
	interpolateValue->transform.position = glm::vec3(167.0f, 5.0f, 1.0f);
	interpolateValue->setColor(Engine::RED);
	interpolateValue->addTag("interpolation_value");

	Engine::Canvas *hudDivider = new Engine::Canvas(glm::vec2(2.0f, 70.0f));
	hudDivider->transform.position = glm::vec3(555.0f, 530.f, 4.0f);
	hudDivider->setColor(Engine::BLACK);

	Engine::TextObject *gameplayTitle = new Engine::TextObject("Gameplay");
	gameplayTitle->transform.position = glm::vec3(320.0f, 25.0f, 1.0f);
	gameplayTitle->setColor(Engine::BLACK);

	Engine::TextObject *scoreTitle = new Engine::TextObject("Puntos:");
	scoreTitle->transform.position = glm::vec3(300.0f, 5.0f, 1.0f);
	scoreTitle->setColor(Engine::BLACK);

	Engine::TextObject *scoreValue = new Engine::TextObject("0");
	scoreValue->transform.position = glm::vec3(337.0f, 5.0f, 1.0f);
	scoreValue->setColor(Engine::ORANGE);
	scoreValue->addTag("score_value");

	Engine::TextObject *timeTitle = new Engine::TextObject("Tiempo:");
	timeTitle->transform.position = glm::vec3(300.0f, 15.0f, 1.0f);
	timeTitle->setColor(Engine::BLACK);

	Engine::TextObject *timeValue = new Engine::TextObject("00:00");
	timeValue->transform.position = glm::vec3(340.0f, 15.0f, 1.0f);
	timeValue->setColor(Engine::ORANGE);
	timeValue->addTag("time_value");
	timeValue->addBehaviour(new TimeController());

	Engine::Canvas *hud = new Engine::Canvas(glm::vec2(800.0f, 70.0f));
	hud->transform.position = glm::vec3(0.0f, 530.f, 3.0f);
	hud->setColor(Engine::GRAY);
	hud->addBehaviour(new HudController());
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

	scene->addGameObject(hudDivider);

	hud->addChild(gameplayTitle);

	hud->addChild(scoreTitle);
	hud->addChild(scoreValue);
	hud->addChild(timeTitle);
	hud->addChild(timeValue);
}
