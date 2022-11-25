#pragma once

#include <string>
#include <vector>
#include <set>
#include <unordered_map>

#include "GameObject.h"
#include "Camera.h"
#include "Light.h"
#include "TextObject.h"

#include "../Renderer/Skybox.h"
#include "../Physics/Collider.h"

#include "../Renderer/MeshRenderer.h"
#include "../Renderer/ShadowRenderer.h"
#include "../Renderer/TextRenderer.h"
#include "../Renderer/ParticleSystem.h"

namespace Engine
{
	class Scene
	{
	private:
		Camera *_activeCamera;
		std::unordered_map<std::string, Camera *> _cameras;
		std::vector<std::string> _cameraNames;
		std::vector<BaseGameObject *> _gameObjects;
		std::vector<BaseGameObject *> _queuedGameObjects;
		std::set<BaseGameObject *> _gameObjectsToDelete;
		std::vector<Collider *> _colliders;
		std::vector<Light *> _lights;
		Skybox *_skybox;
		ParticleSystem *_ps;

		std::unordered_map<std::string, std::vector<std::shared_ptr<MeshRenderer>>> _meshRenderers = {};
		std::unordered_map<std::string, std::vector<std::shared_ptr<ShadowRenderer>>> _shadowRenderers = {};
		std::unordered_map<std::string, std::vector<std::shared_ptr<TextRenderer>>> _textRenderers = {};

	public:
		Scene(Camera *);
		Scene(const Scene *);
		void addGameObject(BaseGameObject *);
		void addCollider(Collider *);
		void removeCollider(Collider *);
		void deleteGameObject(BaseGameObject *);
		void flushQueuedGameObjects();
		BaseGameObject *getGameObjectWithTag(std::string);
		void addCamera(std::string, Camera *);
		void setActiveCamera(std::string);
		Camera *getActiveCamera();
		Camera *getCamera(std::string);
		std::vector<std::string> getCameraNames() const;
		std::vector<Light *> getLights();
		void drawSkybox(glm::mat4 projection, glm::mat4 view);
		void setSkybox(Skybox *);
		void setParticleSystem(ParticleSystem *ps);
		void drawParticles();
		void draw(Shader *);
		void start();
		void update();
		void physicsUpdate();
		void animationsUpdate(float deltaTime);
		void particleSystemUpdate(float deltaTime);
		void addRenderers(BaseGameObject* gameObject);
		void removeRenderers(BaseGameObject* gameObject);

		std::unordered_map<std::string, std::vector<std::shared_ptr<MeshRenderer>>>& getMeshRenderers() { return _meshRenderers; };
		std::unordered_map<std::string, std::vector<std::shared_ptr<ShadowRenderer>>>& getShadowRenderers() { return _shadowRenderers; };
		std::unordered_map<std::string, std::vector<std::shared_ptr<TextRenderer>>>& getTextRenderers() { return _textRenderers; };

		~Scene();
	};
}
