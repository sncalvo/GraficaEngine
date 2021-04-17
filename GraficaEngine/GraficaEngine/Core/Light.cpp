#include <glm/gtc/type_ptr.hpp>

#include "Light.h"

namespace Engine
{
	Light::Light(Transform transform, float intensity, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
		_transform(transform),
		_intensity(intensity),
		_ambient(ambient),
		_diffuse(diffuse),
		_specular(specular)
	{}

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