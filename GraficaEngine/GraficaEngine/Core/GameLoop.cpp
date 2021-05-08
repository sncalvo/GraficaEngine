#include "GameLoop.h"

namespace Engine
{
	GameLoop::GameLoop() : _activeScene(nullptr), _shader(nullptr), _window(nullptr)
	{
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

			// TODO: Pause bullet physics. Ref: https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=6487
			if (_gamePaused || !_activeScene)
			{
				continue;
			}

			Camera *camera = _activeScene->getActiveCamera();

			if (input.getKeyDown(KEY_ESCAPE) || input.getKeyDown(KEY_Q))
				break;

			_shader->use();
			glm::mat4 projection = camera->getProjectionMatrix();
			glm::mat4 view = camera->getViewMatrix();
			_shader->setMat4("projection", projection);
			_shader->setMat4("view", view);

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
