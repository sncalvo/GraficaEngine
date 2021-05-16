#pragma once

#include <vector>

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/GLU.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Scene.h"
#include "GameObject.h"
#include "Input.h"
#include "Time.h"
#include "Camera.h"
#include "../Platform/Window.h"
#include "../Platform/MediaLayer.h"

namespace Engine
{
	constexpr auto PAUSE_KEY = KEY_P;

	class GameLoop
	{
	private:
		GameLoop();
		Window *_window;
		Shader *_shader;
		Scene *_activeScene;
		std::vector<GameObject *> _gameObjects;
		bool _gamePaused;
		void _handleGameSpeed() const;
		void _handleWireframeToggle() const;
		void _handleTexturesToggle() const;
		void _handleShowCollidersToggle() const;

	public:
		static GameLoop getInstance()
		{
			static GameLoop _instance;
			return _instance;
		}
		void start();
		void setActiveScene(Scene *);
		void addWindow(Window *);
		void addShader(Shader *);
	};
}
