#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Engine
{
	class Shader
	{
	public:
		// the program ID
		unsigned int ID;
		Shader();
		// constructor reads and builds the shader
		Shader(const char* vertexPath, const char* fragmentPath);
		// use/activate the shader
		void use();
		// utility uniform functions
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setFloat4f(const std::string& name, float value0, float value1, float value2, float value3) const;
		void setVec2f(const std::string& name, const glm::f32* value) const;
		void setVec2(const std::string& name, float value0, float value1) const;
		void setVec3f(const std::string& name, const glm::f32* value) const;
		void setVec3(const std::string& name, float value0, float value1, float value2) const;
		void setMatrix4f(const std::string& name, glm::f32* matrix) const;
		void setMat4(const std::string& name, const glm::mat4& mat) const;
	};
}
