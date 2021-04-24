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
}