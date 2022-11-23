#include <glm/gtc/type_ptr.hpp>

#include "Light.h"

#include "Settings.h"

namespace Engine
{
	Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction) :
		_transform(),
		_intensity(1.0f),
		_ambient(ambient),
		_diffuse(diffuse),
		_specular(specular),
		_direction(direction)
	{
		setPosition(glm::vec4(10.f, 10.f, 10.f, 1.f));
	}

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

	glm::mat4 Light::getLightSpaceMatrix() const
	{
		// TODO: Make near and far params
		float nearPlane = 10.0f, farPlane = 40.f;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
		glm::mat4 lightView = _transform.getLookAt(glm::vec3{ 0.f, 0.f, 0.f });

		return lightProjection * lightView;
	}

	std::vector<glm::mat4> Light::getLightSpaceMatrices(std::vector<std::vector<glm::vec4>>& corners) const
	{
		Settings& settings = Settings::getInstance();
		std::vector<float> shadowCascadeLevels = settings.getShadowCascadeLevels();

		std::vector<glm::mat4> ret;
		for (size_t i = 0; i < shadowCascadeLevels.size() + 1; ++i)
		{
			ret.push_back(getLightSpaceMatrixCSM(corners[i]));
		}
		return ret;
	}

	glm::mat4 Light::getLightSpaceMatrixCSM(std::vector<glm::vec4> corners) const
	{
		glm::vec3 center = glm::vec3(0, 0, 0);
		for (const auto& v : corners)
		{
			center += glm::vec3(v);
		}
		center /= corners.size();

		const auto lightView = glm::lookAt(
			center + _direction,
			center,
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

		float minX = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::min();
		float minY = std::numeric_limits<float>::max();
		float maxY = std::numeric_limits<float>::min();
		float minZ = std::numeric_limits<float>::max();
		float maxZ = std::numeric_limits<float>::min();
		for (const auto& v : corners)
		{
			const auto trf = lightView * v;
			minX = std::min(minX, trf.x);
			maxX = std::max(maxX, trf.x);
			minY = std::min(minY, trf.y);
			maxY = std::max(maxY, trf.y);
			minZ = std::min(minZ, trf.z);
			maxZ = std::max(maxZ, trf.z);
		}
		// Tune this parameter according to the scene
		constexpr float zMult = 5.f;
		if (minZ < 0)
		{
			minZ *= zMult;
		}
		else
		{
			minZ /= zMult;
		}
		if (maxZ < 0)
		{
			maxZ /= zMult;
		}
		else
		{
			maxZ *= zMult;
		}

		const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

		return lightProjection * lightView;
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
				glm::vec3(1.0f),
				glm::vec3(1.f, 0.5f, 0.f)
			),
		},
		{
			DayTime::MIDDAY,
			new Light(
				glm::vec3(0.4f, 0.8f, 0.8f),
				glm::vec3(0.8f, 1.f, 1.f),
				glm::vec3(1.f),
				glm::normalize(glm::vec3(0.1f, -0.9f, 0.f))
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
