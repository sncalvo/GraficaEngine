#include "RenderPipeline.h"

namespace Engine {
	RenderPipeline::RenderPipeline() : _defaultShader(nullptr), _shadowShader(nullptr)
	{
	}

	void RenderPipeline::setup() {
		_defaultShader = new Shader();
	}

    void RenderPipeline::draw(Scene* scene) {
		Camera* camera = scene->getActiveCamera();
		glm::mat4 projection = camera->getProjectionMatrix();
		glm::mat4 view = camera->getViewMatrix();

		scene->drawSkybox(projection, view);

		// TODO: Draw shadows here to framebuffer

		//_defaultShader->use();
		//_defaultShader->setMat4("projection", projection);
		//_defaultShader->setMat4("view", view);
		
		// Important: auto does not infer references. So auto cannot be used here.
		std::unordered_map<std::string, std::vector<std::shared_ptr<MeshRenderer>>>& meshRenderers = scene->getMeshRenderers();

		auto meshShader = MeshRenderer::getShader();
		meshShader->use();
		meshShader->setMat4("projection", projection);
		meshShader->setMat4("view", view);

		std::vector<Light*> lights = scene->getLights();

		for (Light* light : lights)
		{
			light->apply(*meshShader);
		}

		for (auto& [key, value] : meshRenderers)
		{
			for (auto meshRenderer : value)
			{
				meshRenderer->draw(camera->transform.position);
			}
		}

		scene->draw(_defaultShader);
    }
}
