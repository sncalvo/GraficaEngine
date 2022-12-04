#include "GameLoop.h"

#include "Light.h"
#include "SceneManager.h"
#include "Settings.h"
#include <stdlib.h>
#include <time.h>
#include "../Renderer/ParticleSystem.h"

namespace Engine
{
	GameLoop::GameLoop() :
		_window(nullptr),
		_gamePaused(false),
		_renderer(RenderPipeline())
	{
	}

	void GameLoop::_handleDayTime() const
	{
		Input &input = Input::getInstance();
		Settings &settings = Settings::getInstance();

		if (input.getKeyDown(KEY_O))
		{
			switch (settings.getDayTime())
			{
			case DayTime::MORNING:
				settings.setDayTime(DayTime::MIDDAY);
				break;
			case DayTime::MIDDAY:
				settings.setDayTime(DayTime::AFTERNOON);
				break;
			case DayTime::AFTERNOON:
				settings.setDayTime(DayTime::NIGHT);
				break;
			case DayTime::NIGHT:
				settings.setDayTime(DayTime::MORNING);
				break;
			}
		}
	}

	void GameLoop::_handleShadowMapDebug() const
	{
		Input& input = Input::getInstance();
		Settings& settings = Settings::getInstance();

		if (input.getKeyDown(KEY_LEFT_SHIFT))
		{
			settings.setDebugShadowMap(!settings.getDebugShadowMap());
		}
	}

	void GameLoop::_handleGameSpeed() const
	{
		Input &input = Input::getInstance();
		Settings &settings = Settings::getInstance();

		if (input.getKeyDown(KEY_1))
		{
			settings.setGameSpeed(GameSpeed::SLOW);
		}
		else if (input.getKeyDown(KEY_2))
		{
			settings.setGameSpeed(GameSpeed::NORMAL);
		}
		else if (input.getKeyDown(KEY_3))
		{
			settings.setGameSpeed(GameSpeed::FAST);
		}
	}

	void GameLoop::_handleTexturesToggle() const
	{
		Input &input = Input::getInstance();
		Settings &settings = Settings::getInstance();

		if (input.getKeyDown(KEY_T))
		{
			bool showingTextures = settings.getShowTextures();
			settings.setShowTextures(!showingTextures);
		}
	}

	void GameLoop::_handleShowCollidersToggle() const
	{
		Input &input = Input::getInstance();
		Settings &settings = Settings::getInstance();

		if (input.getKeyDown(KEY_C))
		{
			bool showingColliders = settings.getShowColliders();
			settings.setShowColliders(!showingColliders);
		}
	}

	void GameLoop::_handleBiasModifier() const
	{
		Input& input = Input::getInstance();
		Settings& settings = Settings::getInstance();
		auto biasModifier = settings.getBiasModifier();

		if (input.getKeyDown(KEY_LEFTBRACKET))
		{
			settings.setBiasModifier(biasModifier - 0.01f);
			std::cout << "Bias mod set to " << biasModifier - 0.01f << std::endl;
		}
		else if (input.getKeyDown(KEY_RIGHTBRACKET))
		{
			settings.setBiasModifier(biasModifier + 0.01f);
			std::cout << "Bias mod set to " << biasModifier + 0.01f << std::endl;
		}
	}

	void GameLoop::_handleInterpolationToggle() const
	{
		Input &input = Input::getInstance();
		Settings &settings = Settings::getInstance();

		if (input.getKeyDown(KEY_U))
		{
			bool useInterpolation = settings.getUseInterpolation();
			settings.setUseInterpolation(!useInterpolation);
		}
	}

	void GameLoop::_handleWireframeToggle() const
	{
		Input &input = Input::getInstance();
		Settings &settings = Settings::getInstance();

		if (input.getKeyDown(KEY_Y))
		{
			bool wireframeOn = settings.getIsWireframe();
			settings.setIsWireframe(!wireframeOn);
		}
	}

	void GameLoop::_handleFogToggle() const
	{
		Input& input = Input::getInstance();
		Settings& settings = Settings::getInstance();

		if (input.getKeyDown(KEY_COMMA))
		{
			bool fog = settings.getFog();
			settings.setFog(!fog);

			std::cout << "Toggled fog " << !fog << std::endl;
		}
	}

	void GameLoop::_handleBloomToggle() const
	{
		Input& input = Input::getInstance();
		Settings& settings = Settings::getInstance();

		if (input.getKeyDown(KEY_M))
		{
			bool bloom = settings.getBloom();
			settings.setBloom(!bloom);

			std::cout << "Toggled bloom " << !bloom << std::endl;
		}
	}

	void GameLoop::start()
	{
		Input &input = Input::getInstance();
		SceneManager &sceneManager = SceneManager::getInstance();

		_renderer.setup();

		Scene *activeScene = sceneManager.getActiveScene();
		activeScene->start();

		float frameCounterUpdateThreshold = 0.f;
		unsigned int counter = 0;
		float deltaTimeAcc = 0.f;

        glm::vec3 camera_position = activeScene->getActiveCamera()->transform.position;
        Engine::ParticleSystem *ps = new Engine::ParticleSystem(camera_position);
		activeScene->setParticleSystem(ps);

		while (true)
		{
			counter++;

			Time::updateTime();
			input.update();
			frameCounterUpdateThreshold += Time::getDeltaTime();

			if (frameCounterUpdateThreshold > 1.f)
			{
				std::string FPS = std::to_string((1.f / frameCounterUpdateThreshold) * counter);
				std::string ms = std::to_string((frameCounterUpdateThreshold / counter) * 100);
				std::string title = "GraficaEngine - " + FPS + " FPS / " + ms + " ms";
				_window->setTitle(title);
				counter = 0;
				frameCounterUpdateThreshold = 0.f;
			}		

			if (input.getKeyDown(KEY_Q))
				break;

			if (input.getKeyDown(PAUSE_KEY))
			{
				_gamePaused = !_gamePaused;
			}

			if (_gamePaused || !activeScene)
			{
				continue;
			}


			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glPolygonMode(GL_FRONT_AND_BACK, Settings::getInstance().getIsWireframe() ? GL_LINE : GL_FILL);

			_handleGameSpeed();
			_handleTexturesToggle();
			_handleShowCollidersToggle();
			_handleWireframeToggle();
			_handleInterpolationToggle();
			_handleDayTime();
			_handleShadowMapDebug();
			_handleFogToggle();
			_handleBloomToggle();
			_handleBiasModifier();

			if (sceneManager.getShouldRestart())
			{
				sceneManager.loadScene("main");
				sceneManager.setShouldRestart(false);
				activeScene = sceneManager.getActiveScene();
			}

			activeScene->physicsUpdate();
			if (counter % 2 == 0) {
				activeScene->animationsUpdate(deltaTimeAcc);
				deltaTimeAcc = 0.f;
			} else {
				deltaTimeAcc += Time::getDeltaTime();
			}
			activeScene->update();
			activeScene->particleSystemUpdate(Time::getDeltaTime());
			activeScene->flushQueuedGameObjects();

			_renderer.draw(activeScene);

			_window->swap();
		}

		for (GameObject *gameObject : _gameObjects)
		{
			delete gameObject;
		}

		// delete sceneManager;
		delete _window;
		FontManager *fm = FontManager::getInstance();
		delete fm;
		MediaLayer::exit();
	}

	void GameLoop::addWindow(Window *window)
	{
		_window = window;
	}
}
