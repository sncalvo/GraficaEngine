#include "GameLoop.h"

namespace Engine
{
	GameLoop::GameLoop() {}

	void GameLoop::start()
	{
		Input& input = Input::getInstance();

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

		while (true)
		{
			Time::updateTime();
			input.update();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			int movementX, movementY;
			std::tie(movementX, movementY) = input.getMouseMovement();

			_camera->processMouseMovement(float(movementX) * Engine::Time::getDeltaTime(), float(movementY) * Engine::Time::getDeltaTime());
			if (input.getKey(Engine::KEY_W))
			{
				_camera->processKeyboard(Engine::Camera_Movement::FORWARD, Engine::Time::getDeltaTime());
			}
			if (input.getKey(Engine::KEY_S))
			{
				_camera->processKeyboard(Engine::Camera_Movement::BACKWARD, Engine::Time::getDeltaTime());
			}
			if (input.getKey(Engine::KEY_A))
			{
				_camera->processKeyboard(Engine::Camera_Movement::LEFT, Engine::Time::getDeltaTime());
			}
			if (input.getKey(Engine::KEY_D))
			{
				_camera->processKeyboard(Engine::Camera_Movement::RIGHT, Engine::Time::getDeltaTime());
			}

			if (input.getKey(KEY_ESCAPE))
				break;

			_shader->use();
			glm::mat4 projection = glm::perspective(glm::radians(_camera->getZoom()), 800.0f / 600.0f, 0.1f, 100.0f);
			glm::mat4 view = _camera->getViewMatrix();
			_shader->setMat4("projection", projection);
			_shader->setMat4("view", view);

			for (GameObject* gameObject : _gameObjects)
			{
				gameObject->update();
			}

			for (GameObject* gameObject : _gameObjects)
			{
				gameObject->draw();
			}

			_window->swap();
		}

		for (GameObject* gameObject : _gameObjects)
		{
			delete gameObject;
		}

		delete _shader;
		delete _camera;
		delete _window;
		MediaLayer::exit();
	}

	void GameLoop::addCamera(Camera *camera)
	{
		_camera = camera;
	}

	void GameLoop::addWindow(Window* window)
	{
		_window = window;
	}

	void GameLoop::addShader(Shader *shader)
	{
		_shader = shader;
	}

	void GameLoop::addGameObject(GameObject *gameObject)
	{
		_gameObjects.push_back(gameObject);
	}
}