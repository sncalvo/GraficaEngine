#pragma once

#include "BaseGameObject.h"

#include <glm/glm.hpp>

namespace Engine {
	class Canvas : public BaseGameObject
	{
	private:
		Shader* _shader = nullptr;
		unsigned int _VAO, _VBO = 0;
		glm::vec3 _color;
		glm::vec2 _size;
	public:
		Canvas(glm::vec2 size, glm::vec3 color = glm::vec3(1.f));
		Canvas(const Canvas *);
		void setSize(glm::vec2 size);
		glm::vec2 getSize();
		void setColor(glm::vec3 color);
		glm::vec3 getColor();
		void draw() const;
		Canvas *clone() const override;
	};
}
