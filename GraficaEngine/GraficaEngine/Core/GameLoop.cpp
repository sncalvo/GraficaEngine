#include "GameLoop.h"

namespace Engine
{
	GameLoop::GameLoop(): _activeScene(nullptr), _shader(nullptr), _window(nullptr)
	{}

	void GameLoop::start()
	{
		Input& input = Input::getInstance();

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

		while (true)
		{
			Time::updateTime();
			input.update();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (!_activeScene)
			{
				continue;
			}

			Camera* camera = _activeScene->getCamera();

			if (input.getKeyDown(KEY_ESCAPE) || input.getKeyDown(KEY_Q))
				break;

			_shader->use();
			glm::mat4 projection = glm::perspective(glm::radians(camera->getZoom()), 800.0f / 600.0f, 0.1f, 100.0f);
			glm::mat4 view = camera->getViewMatrix();
			_shader->setMat4("projection", projection);
			_shader->setMat4("view", view);

			_activeScene->update();
			_activeScene->draw();

			_window->swap();
		}

		for (GameObject* gameObject : _gameObjects)
		{
			delete gameObject;
		}

		delete _shader;
		delete _window;
		MediaLayer::exit();
	}

	void GameLoop::setActiveScene(Scene* scene)
	{
		if (_activeScene)
		{
			delete _activeScene;
		}

		_activeScene = scene;
	}

	void GameLoop::addWindow(Window* window)
	{
		_window = window;
	}

	void GameLoop::addShader(Shader *shader)
	{
		_shader = shader;
	}
}