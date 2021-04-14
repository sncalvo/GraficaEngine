#pragma once

#include <vector>
#include <SDL/SDL_opengl.h>

#include <glm/vec4.hpp>

#include "Drawable.h"
#include "Transform.h"

class Light: public Drawable
{
private:
	Transform _transform;

	glm::vec3 _ambient;
	glm::vec3 _diffuse;
	glm::vec3 _specular;
	float _intensity;
public:
	Light(Transform transform, float intensity, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	virtual void draw() const = 0;
	void setIntensity(float intensity);
	void setPosition(glm::vec4 position);
	void setAmbient(glm::vec4 color);
	void setDiffuse(glm::vec4 color);
	void setSpecular(glm::vec4 color);
};
