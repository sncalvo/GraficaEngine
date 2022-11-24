#include "GameLoop.h"

#include "Light.h"
#include "SceneManager.h"
#include "Settings.h"
#include <stdlib.h>
#include <time.h>

namespace Engine
{
	struct Particle {
		int position_id;
		glm::vec3 velocity;
		glm::vec4 color;
		float     life;
	
		Particle(int position_id) 
		: position_id(position_id), velocity(glm::vec3(-0.0001f, -0.0005f, 0.f)), color(1.f), life(0.f) { }
	};  

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

    float parabola(float t) {
	  return - 0.2f * t * t;
	}

	float rand11() {
		return (rand() / (float)RAND_MAX) * 2 - 1;
	}
	float rand01() {
		return ((double)rand() / RAND_MAX) * 1.01 + 0.01;
	}

	void GameLoop::start()
	{
		srand((unsigned)time(0));

		Input &input = Input::getInstance();
		SceneManager &sceneManager = SceneManager::getInstance();

		float frameCounterUpdateThreshold = 0.f;
		unsigned int counter = 0;
		float initial_height = 1.0;
        int particle_amount = 10000;

		std::vector<Particle> particles;
		std::vector<float> vertices;
		vertices.reserve(3 * particle_amount);
		particles.reserve(particle_amount);
		for(int i = 0; i < particle_amount; i++) {
			int position_id = i * 3;
			particles.push_back(Particle(position_id));
			vertices.push_back(rand11());
			vertices.push_back(rand11());
			vertices.push_back(rand01() * -1);
		}

		unsigned int VAO;
		glGenVertexArrays(1, &VAO); 
		glBindVertexArray(VAO);

		unsigned int VBO;
		glGenBuffers(1, &VBO);  
		glBindBuffer(GL_ARRAY_BUFFER, VBO);  
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);  
		Engine::Shader shader("Assets/Shaders/particles.vs", "Assets/Shaders/particles.gs", "Assets/Shaders/particles.fs");
		Engine::Texture texture("Assets/Models/shape-white.png", "lol");
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable( GL_BLEND );
		auto projection = glm::perspective(glm::radians(150.f), 1.f, 0.0f, 1.f); // TODO: Change parameters

		while (true)
		{
			counter++;

			Time::updateTime();
			input.update();

			for(auto particle : particles) {
				int vertices_position = particle.position_id;
				vertices[vertices_position] += particle.velocity.x;
				vertices[vertices_position + 1] += particle.velocity.y;
				vertices[vertices_position + 2] += particle.velocity.z;
			}

			glBindBuffer(GL_ARRAY_BUFFER, VBO);  
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

			std::cout << (float)(rand01() * -1) << std::endl;

			frameCounterUpdateThreshold += Time::getDeltaTime();
			
			if (input.getKeyDown(KEY_Q))
				break;

			glClearColor(0.0,0.0,0.0,1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glPolygonMode(GL_FRONT_AND_BACK, Settings::getInstance().getIsWireframe() ? GL_LINE : GL_FILL);

            shader.use();

			shader.setMat4("projection", projection);
			texture.activateTextureAs(0);
			glBindVertexArray(VAO);
			glDrawArrays(GL_POINTS, 0, particle_amount);
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
