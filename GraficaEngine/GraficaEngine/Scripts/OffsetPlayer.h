#pragma once

#include <glm/glm.hpp>

#include "../Core/Behaviour.h"
#include "../Core/Scene.h"

class OffsetPlayer : public Engine::Behaviour
{
private:
	glm::vec3 _offset;

public:
	OffsetPlayer(glm::vec3 offset);
	void update();
	OffsetPlayer *clone() const override;
};
