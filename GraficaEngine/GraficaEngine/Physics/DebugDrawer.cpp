#include "DebugDrawer.h"

namespace Engine
{
	void DebugDrawer::setMatrices(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
	{
		auto shader = getShader();

		shader->use();
		shader->setMat4("view", viewMatrix);
		shader->setMat4("projection", projectionMatrix);
	}

	void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		LineValues line;
		line.fromX = from.getX();
		line.fromY = from.getY();
		line.fromZ = from.getZ();
		line.toX = to.getX();
		line.toY = to.getY();
		line.toZ = to.getZ();
		_lines.push_back(line);
	}

	void DebugDrawer::doDebugDraw()
	{
		glDeleteBuffers(1, &_vbo);
		glDeleteVertexArrays(1, &_vao);

		glGenBuffers(1, &_vbo);
		glGenVertexArrays(1, &_vao);

		glBindVertexArray(_vao);

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, _lines.size() * sizeof(float) * 6, &_lines[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindVertexArray(0);

		glEnable(GL_LINE_WIDTH);
		glLineWidth(10.);

		glBindVertexArray(_vao);
		glDrawArrays(GL_LINES, 0, _lines.size() * 2);
		glBindVertexArray(0);

		_lines.clear();
	}

	void DebugDrawer::setDebugMode(int debugMode)
	{
		_debugMode = (DebugDrawModes) debugMode;
	}

	int DebugDrawer::getDebugMode() const
	{
		return (int)_debugMode;
	}
}