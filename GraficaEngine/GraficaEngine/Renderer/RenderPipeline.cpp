#include "RenderPipeline.h"

#include <glm/gtc/type_ptr.hpp>

void renderQuad();

#include "./Shader.h"

#include "../Core/Settings.h"

Engine::Shader* testShader = nullptr;
unsigned int depthMap = 0;
unsigned int depthMapFBO;
unsigned int matricesUBO;

unsigned int hdrFBO;
unsigned int colorBuffers[2];

unsigned int quadVAOHDR = 0;
unsigned int quadVBOHDR = 0;

unsigned int pingpongFBO[2];
unsigned int pingpongBuffers[2];

namespace Engine {
	RenderPipeline::RenderPipeline() : _defaultShader(nullptr), _shadowShader(nullptr), _shaderBlur(nullptr)
	{
	}

	void RenderPipeline::setup() {
		unsigned int shadowResolution = Settings::getInstance().getShadowResolution();
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glEnable(GL_CULL_FACE);
		testShader = new Engine::Shader("Assets/Shaders/test.vs", "Assets/Shaders/test.fs");
		_hdrShader = new Engine::Shader("Assets/Shaders/test.vs", "Assets/Shaders/hdrDraw.fs");
		_shaderBlur = new Engine::Shader("Assets/Shaders/test.vs", "Assets/Shaders/blur.fs");

		_shaderBlur->use();
		_shaderBlur->setInt("image", 0);

		glGenFramebuffers(1, &depthMapFBO);
		// TODO: Change to shadow mapping resolution
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D_ARRAY, depthMap);
		glTexImage3D(
			GL_TEXTURE_2D_ARRAY,
			0,
			GL_DEPTH_COMPONENT32F,
			shadowResolution,
			shadowResolution,
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

		glGenFramebuffers(1, &hdrFBO);

		// create depth buffer (renderbuffer)
		// TODO: We will need to destroy and regenerate this if we change window size
		GLuint rboDepth;
		unsigned int width, height;
		std::tie(width, height) = Settings::getInstance().getWindowSize();
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

		// attach buffers
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

		glGenTextures(2, colorBuffers);
		for (unsigned int i = 0; i < 2; i++)
		{
			glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL
			);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glGenerateMipmap(GL_TEXTURE_2D);
			// attach texture to framebuffer
			glFramebufferTexture2D(
				GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0
			);
		}
		unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, attachments);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Generate framebuffers for bloom effect
		glGenFramebuffers(2, pingpongFBO);
		glGenTextures(2, pingpongBuffers);
		for (unsigned int i = 0; i < 2; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
			glBindTexture(GL_TEXTURE_2D, pingpongBuffers[i]);
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL
			);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(
				GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffers[i], 0
			);
		}
	}

    void RenderPipeline::draw(Scene* scene, Animator *a) {
		Camera* camera = scene->getActiveCamera();
		glm::mat4 projection = camera->getProjectionMatrix();
		glm::mat4 view = camera->getViewMatrix();

		// Inverting faces since to avoid shadow bias
		glCullFace(GL_FRONT);
		_drawShadows(scene);
		glCullFace(GL_BACK);

<<<<<<< Updated upstream
		_drawMeshes(scene);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		_drawFrame();

		glClear(GL_DEPTH_BUFFER_BIT);

=======
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		scene->drawSkybox(projection, view);
		_drawMeshes(scene, a);
>>>>>>> Stashed changes
		renderQuad();
    }

	void RenderPipeline::_drawShadows(Scene* scene)
	{
		auto shader = ShadowRenderer::getShader();

		shader->use();

		// Important: auto does not infer references. So auto cannot be used here.
		std::unordered_map<std::string, std::vector<std::shared_ptr<ShadowRenderer>>>& shadowRenderers = scene->getShadowRenderers();

		unsigned int shadowResolution = Settings::getInstance().getShadowResolution();
		glViewport(0, 0, shadowResolution, shadowResolution);

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

			for (auto& [key, value] : shadowRenderers)
			{
				for (auto shadowRenderer : value)
				{
					shadowRenderer->draw();

				}
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderPipeline::_drawMeshes(Scene* scene, Animator *a)
	{
		unsigned int width, height;
		std::tie(width, height) = Settings::getInstance().getWindowSize();

		glViewport(0, 0, width, height);

		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		Camera* camera = scene->getActiveCamera();
		glm::mat4 projection = camera->getProjectionMatrix();
		glm::mat4 view = camera->getViewMatrix();

		scene->drawSkybox(projection, view);

		std::unordered_map<std::string, std::vector<std::shared_ptr<MeshRenderer>>>& meshRenderers = scene->getMeshRenderers();

		auto meshShader = MeshRenderer::getShader();
		meshShader->use();
		meshShader->setMat4("projection", projection);
		meshShader->setMat4("view", view);
		auto transforms = a->GetFinalBoneMatrices();

/*         for (int i = 0; i < transforms.size(); ++i)
            meshShader->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]); */

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

		for (auto& [key, value] : meshRenderers)
		{
			for (auto meshRenderer : value)
			{
				meshRenderer->draw(depthMap);
			}
		}

		scene->draw(_defaultShader);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderPipeline::_drawFrame()
	{
		if (quadVAOHDR == 0)
		{
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &quadVAOHDR);
			glGenBuffers(1, &quadVBOHDR);
			glBindVertexArray(quadVAOHDR);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBOHDR);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}

		bool horizontal = true, first_iteration = true;
		int amount = 10;
		_shaderBlur->use();
		for (unsigned int i = 0; i < amount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
			_shaderBlur->setInt("horizontal", horizontal);
			glBindTexture(
				GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongBuffers[!horizontal]
			);
			glBindVertexArray(quadVAOHDR);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		unsigned int exposure = Settings::getInstance().getExposure();
		_hdrShader->use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffers[!horizontal]);

		_hdrShader->setFloat("exposure", exposure);
		_hdrShader->setInt("colorBuffer", 0);
		_hdrShader->setInt("bloomBlur", 1);

		glBindVertexArray(quadVAOHDR);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}
}

unsigned int quadVAO = 0;
unsigned int quadVBO = 0;

void renderQuad()
{
	testShader->use();

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
