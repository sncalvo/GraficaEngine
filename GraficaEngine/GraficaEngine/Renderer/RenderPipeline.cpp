#include "RenderPipeline.h"

#include <glm/gtc/type_ptr.hpp>

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

void renderQuad();

#include "./Shader.h"

Engine::Shader* testShader = nullptr;
unsigned int depthMap = 0;

namespace Engine {
	RenderPipeline::RenderPipeline() : _defaultShader(nullptr), _shadowShader(nullptr)
	{
	}

	void RenderPipeline::setup() {
		_defaultShader = new Shader();
		testShader = new Engine::Shader("Assets/Shaders/test.vs", "Assets/Shaders/test.fs");
	}

    void RenderPipeline::draw(Scene* scene) {
		Camera* camera = scene->getActiveCamera();
		glm::mat4 projection = camera->getProjectionMatrix();
		glm::mat4 view = camera->getViewMatrix();

		// Inverting faces since to avoid shadow bias
		glCullFace(GL_FRONT);
		_drawShadows(scene);
		glCullFace(GL_BACK);

		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		scene->drawSkybox(projection, view);
		_drawMeshes(scene);
		renderQuad();
    }

	void RenderPipeline::_drawShadows(Scene* scene)
	{
		auto shader = ShadowRenderer::getShader();

		shader->use();

		// Important: auto does not infer references. So auto cannot be used here.
		std::unordered_map<std::string, std::vector<std::shared_ptr<ShadowRenderer>>>& shadowRenderers = scene->getShadowRenderers();

		unsigned int depthMapFBO;
		glGenFramebuffers(1, &depthMapFBO);
		// TODO: Change to shadow mapping resolution
		const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		std::vector<Light*> lights = scene->getLights();
		auto frustumCorners = scene->getActiveCamera()->getFrustumCornersWorldSpace();

		// TODO: We are assumming only one light for now. We should create a map for each light
		for (Light* light : lights)
		{
			auto lightSpaceMatrix = light->getLightSpaceMatrix();
			shader->setMatrix4f("lightSpaceMatrix", glm::value_ptr(lightSpaceMatrix));

			for (auto& [key, value] : shadowRenderers)
			{
				for (auto shadowRenderer : value)
				{
					shadowRenderer->draw();
				}
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		// GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS is arbitrary. We should probably think of a way to track active textures
	}

	void RenderPipeline::_drawMeshes(Scene* scene)
	{
		Camera* camera = scene->getActiveCamera();
		glm::mat4 projection = camera->getProjectionMatrix();
		glm::mat4 view = camera->getViewMatrix();

		std::unordered_map<std::string, std::vector<std::shared_ptr<MeshRenderer>>>& meshRenderers = scene->getMeshRenderers();

		auto meshShader = MeshRenderer::getShader();
		meshShader->use();
		meshShader->setMat4("projection", projection);
		meshShader->setMat4("view", view);

		std::vector<Light*> lights = scene->getLights();

		for (Light* light : lights)
		{
			light->apply(*meshShader);
			auto lightSpaceMatrix = light->getLightSpaceMatrix();
			meshShader->setMatrix4f("lightSpaceMatrix", glm::value_ptr(lightSpaceMatrix));
		}
		meshShader->setVec3f("viewPos", glm::value_ptr(camera->transform.position));

		/*glActiveTexture(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - 1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		meshShader->setInt("shadowMap", GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - 1);*/

		for (auto& [key, value] : meshRenderers)
		{
			for (auto meshRenderer : value)
			{
				meshRenderer->draw(depthMap);
			}
		}

		scene->draw(_defaultShader);
	}
}

unsigned int quadVAO = 0;
unsigned int quadVBO = 0;

void renderQuad()
{
	testShader->use();

	glActiveTexture(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - 1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	testShader->setInt("shadowMap", GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - 1);
	float nearPlane = 1.0f, farPlane = 40.f;
	testShader->setFloat("nearPlane", nearPlane);
	testShader->setFloat("farPlane", farPlane);

	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f, -0.4f, 0.0f,  0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
			-0.4f, -0.4f, 0.0f,  1.0f, 1.0f,
			-0.4f, -1.0f, 0.0f,  1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}