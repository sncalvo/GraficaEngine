#pragma once

#include <vector>
#include <map>

#include <GL/glew.h>
#include <SDL/SDL_opengl.h>

#include <glm/vec4.hpp>

#include "Transform.h"
#include "../Renderer/Shader.h"

namespace Engine
{
	enum class DayTime
	{
		MORNING,
		MIDDAY,
		AFTERNOON,
		NIGHT,
	};
	
	class Light
	{
	private:
		Transform _transform;

		glm::vec3 _ambient;
		glm::vec3 _diffuse;
		glm::vec3 _specular;

		glm::vec3 _direction;
		float _intensity;

	public:
		Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction);
		Light(Transform transform, float intensity, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction);
		Light(const Light *);
		void apply(Shader&) const;
		void setIntensity(float intensity);
		void setPosition(glm::vec4 position);
		void setAmbient(glm::vec4 color);
		void setDiffuse(glm::vec4 color);
		void setSpecular(glm::vec4 color);
		std::vector<glm::mat4> getLightSpaceMatrices(std::vector<std::vector<glm::vec4>>& corners) const;
		glm::mat4 getLightSpaceMatrix() const;
		glm::mat4 getLightSpaceMatrixCSM(std::vector<glm::vec4> corners) const;

		static std::map<DayTime, Light *> lightPresets;
	};
}
