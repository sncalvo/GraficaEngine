#include "RenderPipeline.h"

#include <glm/gtc/type_ptr.hpp>

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

void renderQuad();

#include "./Shader.h"

#include "../Core/Settings.h"

Engine::Shader* testShader = nullptr;
unsigned int depthMap = 0;
unsigned int depthMapFBO;
unsigned int matricesUBO;
const unsigned int SHADOW_RESOLUTION = 2048;

namespace Engine {
	RenderPipeline::RenderPipeline() : _defaultShader(nullptr), _shadowShader(nullptr)
	{
	}

	void RenderPipeline::setup() {
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glEnable(GL_CULL_FACE);
		testShader = new Engine::Shader("Assets/Shaders/test.vs", "Assets/Shaders/test.fs");

		glGenFramebuffers(1, &depthMapFBO);
		// TODO: Change to shadow mapping resolution
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D_ARRAY, depthMap);
		glTexImage3D(
			GL_TEXTURE_2D_ARRAY,
			0,
			GL_DEPTH_COMPONENT32F,
			SHADOW_RESOLUTION,
			SHADOW_RESOLUTION,
			// TODO: Probably not 3
			int(3) + 1,
			0,
			GL_DEPTH_COMPONENT,
			GL_FLOAT,
			nullptr);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
			throw 0;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		auto shadowShader = ShadowRenderer::getShader();
		unsigned int uniformBlockIndexLightSpaceMatrixShadow = glGetUniformBlockIndex(shadowShader->ID, "LightSpaceMatrices");
		glUniformBlockBinding(shadowShader->ID, uniformBlockIndexLightSpaceMatrixShadow, 0);

		auto meshShader = MeshRenderer::getShader();
		unsigned int uniformBlockIndexLightSpaceMatrixMesh = glGetUniformBlockIndex(meshShader->ID, "LightSpaceMatrices");
		glUniformBlockBinding(meshShader->ID, uniformBlockIndexLightSpaceMatrixMesh, 0);

		glGenBuffers(1, &matricesUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4x4) * 16, nullptr, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, matricesUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
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
		
		glViewport(0, 0, SHADOW_RESOLUTION, SHADOW_RESOLUTION);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		std::vector<Light*> lights = scene->getLights();
		Settings& settings = Settings::getInstance();

		std::vector<std::vector<glm::vec4>> corners{};

		std::vector<float> shadowCascadeLevels = settings.getShadowCascadeLevels();
		float cameraNearPlane, cameraFarPlane;
		std::tie(cameraNearPlane, cameraFarPlane) = settings.getCameraNearAndFarPlane();

		for (size_t i = 0; i < shadowCascadeLevels.size() + 1; ++i)
		{
			std::vector<glm::vec4> frustumCorners;
			if (i == 0)
			{
				frustumCorners = scene->getActiveCamera()->getFrustumCornersWorldSpace(cameraNearPlane, shadowCascadeLevels[i]);
			}
			else if (i < shadowCascadeLevels.size())
			{
				frustumCorners = scene->getActiveCamera()->getFrustumCornersWorldSpace(shadowCascadeLevels[i - 1], shadowCascadeLevels[i]);
			}
			else
			{
				frustumCorners = scene->getActiveCamera()->getFrustumCornersWorldSpace(shadowCascadeLevels[i - 1], cameraFarPlane);
			}

			corners.push_back(frustumCorners);
		}

		// TODO: We are assumming only one light for now. We should create a map for each light
		for (Light* light : lights)
		{
			auto lightSpaceMatrices = light->getLightSpaceMatrices(corners);
			glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
			for (size_t i = 0; i < lightSpaceMatrices.size(); ++i)
			{
				glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(glm::mat4x4), sizeof(glm::mat4x4), &lightSpaceMatrices[i]);
			}
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			//shader->setMatrix4f("lightSpaceMatrix", glm::value_ptr(lightSpaceMatrices));

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
		auto cameraRange = Settings::getInstance().getCameraNearAndFarPlane();
		meshShader->setFloat("farPlane", cameraRange.second);

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