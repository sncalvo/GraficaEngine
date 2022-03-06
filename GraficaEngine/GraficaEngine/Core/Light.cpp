#include <glm/gtc/type_ptr.hpp>

#include "Light.h"

namespace Engine
{
	Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction) :
		_transform(),
		_intensity(1.0f),
		_ambient(ambient),
		_diffuse(diffuse),
		_specular(specular),
		_direction(direction)
	{}

	Light::Light(
		Transform transform,
		float intensity,
		glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular,
		glm::vec3 direction
	) :
		_transform(transform),
		_intensity(intensity),
		_ambient(ambient),
		_diffuse(diffuse),
		_specular(specular),
		_direction(direction)
	{}

	Light::Light(const Light *otherLight) :
		_transform(otherLight->_transform),
		_intensity(otherLight->_intensity),
		_ambient(otherLight->_ambient),
		_diffuse(otherLight->_diffuse),
		_specular(otherLight->_specular),
		_direction(otherLight->_direction)
	{
	}

	void Light::apply(Shader& shader) const
	{
		shader.setVec3f("light.diffuse", glm::value_ptr(_diffuse));
		shader.setVec3f("light.ambient", glm::value_ptr(_ambient));
		shader.setVec3f("light.specular", glm::value_ptr(_specular));
		shader.setVec3f("light.direction", glm::value_ptr(_direction));
	}

	void Light::setIntensity(float intensity)
	{
		_intensity = intensity;
	}

	void Light::setPosition(glm::vec4 position)
	{
		_transform.position = position;
	}

	void Light::setAmbient(glm::vec4 color)
	{
		_ambient = color;
	}

	void Light::setDiffuse(glm::vec4 color)
	{
		_diffuse = color;
	}

	void Light::setSpecular(glm::vec4 color)
	{
		_specular = color;
	}

	std::map<DayTime, Light *> Light::lightPresets{
		{
			DayTime::MORNING,
			new Light(
				glm::vec3(0.6f, 0.3f, 0.2f),
				glm::vec3(1.f, 0.7f, 0.2f),
				glm::vec3(1.5f),
				glm::vec3(1.f, 0.5f, 0.f)
			),
		},
		{
			DayTime::MIDDAY,
			new Light(
				glm::vec3(0.7f, 0.8f, 0.8f),
				glm::vec3(0.8f, 1.f, 1.f),
				glm::vec3(1.5f),
				glm::vec3(1.f)
			),
		},
		{
			DayTime::AFTERNOON,
			new Light(
				glm::vec3(0.7f, 0.4f, 0.2f),
				glm::vec3(1.f, 0.7f, 0.2f),
				glm::vec3(1.5f),
				glm::vec3(-1.f, 0.5f, 0.f)
			),
		},
		{
			DayTime::NIGHT,
			new Light(
				glm::vec3(0.f, 0.2f, 0.4f),
				glm::vec3(0.1f, 0.4f, 0.8f),
				glm::vec3(1.5f),
				glm::vec3(1.f)
			),
		},
	};
}
