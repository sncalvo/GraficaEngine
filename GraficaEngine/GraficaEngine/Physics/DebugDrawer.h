#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include <GL/glew.h>
#include <Bullet/LinearMath/btIDebugDraw.h>

#include "../Renderer/Shader.h"

namespace Engine
{
	class DebugDrawer : public btIDebugDraw
	{
	private:
		unsigned int _vao, _vbo;
		DebugDrawModes _debugMode;
		struct {
			float fromX, fromY, fromZ;
			float toX, toY, toZ;
		} typedef LineValues;
		std::vector<LineValues> _lines;
	public:
		void setMatrices(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
		void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
		void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override {};
		void reportErrorWarning(const char* warningString) override {};
		void draw3dText(const btVector3& location, const char* textString) override {};
		void setDebugMode(int debugMode) override;
		int getDebugMode() const override;
		static std::shared_ptr<Shader> getShader()
		{
			static std::shared_ptr<Shader> _shader = std::make_shared<Shader>("Assets/Shaders/line.vs", "Assets/Shaders/line.fs");
			return _shader;
		}
		void doDebugDraw();
	};
}