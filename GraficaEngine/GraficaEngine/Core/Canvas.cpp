#include "Canvas.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <gl/glew.h>

namespace Engine {

	Canvas::Canvas(glm::vec2 size, glm::vec3 color)
	{
		_size = size;
		_color = color;
		_shader = new Shader("Assets/Shaders/canvas.vs", "Assets/Shaders/canvas.fs");

		glm::vec2* vertices = new glm::vec2[4];
		vertices[0].x = 0;
		vertices[0].y = size.y;

		vertices[1].x = 0;
		vertices[1].y = 0;

		vertices[2].x = size.x;
		vertices[2].y = 0;

		vertices[3] = size;

		unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };

		glGenVertexArrays(1, &_VAO);
		glGenBuffers(1, &_VBO);
		glBindVertexArray(_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 4, vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		unsigned int EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	Canvas::Canvas(const Canvas *otherCanvas) :
		_shader(otherCanvas->_shader),
		_VAO(otherCanvas->_VAO),
		_VBO(otherCanvas->_VBO),
		_color(otherCanvas->_color),
		_size(otherCanvas->_size),
		BaseGameObject(otherCanvas)
	{
	}

	Canvas *Canvas::clone() const
	{
		return new Canvas(this);
	}

	void Canvas::setSize(glm::vec2 size)
	{
		_size = size;
	}

	glm::vec2 Canvas::getSize()
	{
		return _size;
	}

	void Canvas::setColor(glm::vec3 color)
	{
		_color = color;
	}

	glm::vec3 Canvas::getColor()
	{
		return _color;
	}

	void Canvas::draw(Shader *shader) const
	{
		_shader->use();

		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(600), -5.0f, 5.0f); //TODO Configure with viewport's size
		_shader->setMatrix4f("projection", glm::value_ptr(projection));

		glBindVertexArray(_VAO);
		auto model = transform.getTransformedModel();
		_shader->setMatrix4f("model", glm::value_ptr(model));
		_shader->setVec3f("color", glm::value_ptr(_color));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		BaseGameObject::draw(shader);
	}
}
