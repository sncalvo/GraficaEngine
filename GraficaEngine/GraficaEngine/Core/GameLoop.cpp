#include "GameLoop.h"

#include "Settings.h"

namespace Engine
{
	GameLoop::GameLoop() : _activeScene(nullptr), _shader(nullptr), _window(nullptr), _gamePaused(false)
	{
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

	void GameLoop::start()
	{
		Input &input = Input::getInstance();

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glEnable(GL_CULL_FACE);

		while (true)
		{
			Time::updateTime();
			input.update();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (input.getKeyDown(PAUSE_KEY))
			{
				_gamePaused = !_gamePaused;
			}

			_handleGameSpeed();
			_handleTexturesToggle();

			if (_gamePaused || !_activeScene)
			{
				continue;
			}

			Camera *camera = _activeScene->getActiveCamera();

			if (input.getKeyDown(KEY_Q))
				break;

			_shader->use();
			glm::mat4 projection = camera->getProjectionMatrix();
			glm::mat4 view = camera->getViewMatrix();
			_shader->setMat4("projection", projection);
			_shader->setMat4("view", view);

			_activeScene->physicsUpdate();
			_activeScene->update();
			_activeScene->draw();

			_window->swap();
		}

		for (GameObject *gameObject : _gameObjects)
		{
			delete gameObject;
		}

		delete _activeScene;
		delete _shader;
		delete _window;
		FontManager* fm = FontManager::getInstance();
		delete fm;
		MediaLayer::exit();
	}

	void GameLoop::setActiveScene(Scene *scene)
	{
		if (_activeScene)
		{
			delete _activeScene;
		}

		_activeScene = scene;
	}

	void GameLoop::addWindow(Window *window)
	{
		_window = window;
	}
	void GameLoop::addShader(Shader *shader)
	{
		_shader = shader;
	}
}
