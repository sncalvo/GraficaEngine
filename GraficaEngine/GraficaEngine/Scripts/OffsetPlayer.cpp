#include "OffsetPlayer.h"

OffsetPlayer::OffsetPlayer(glm::vec3 offset): _offset(offset)
{}

void OffsetPlayer::update()
{
	Engine::BaseGameObject* player = gameObject->getScene()->getGameObjectWithTag("player");
	gameObject->transform.position = player->transform.position + _offset;
}
