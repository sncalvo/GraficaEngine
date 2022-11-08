#include "Renderer.h"

namespace Engine {
	Renderer::Renderer() : _defaultShader(nullptr), _shadowShader(nullptr)
	{
	}

	void Renderer::setup() {
		_defaultShader = new Shader();
	}

    void Renderer::draw(Scene* scene) {
		Camera* camera = scene->getActiveCamera();
		glm::mat4 projection = camera->getProjectionMatrix();
		glm::mat4 view = camera->getViewMatrix();

		scene->drawSkybox(projection, view);

		// TODO: Draw shadows here to framebuffer

		_defaultShader->use();
		_defaultShader->setMat4("projection", projection);
		_defaultShader->setMat4("view", view);

		scene->draw(_defaultShader);
    }
}
