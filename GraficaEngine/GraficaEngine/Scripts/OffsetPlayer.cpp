#include "OffsetPlayer.h"

OffsetPlayer::OffsetPlayer(glm::vec3 offset): _offset(offset)
{}

void OffsetPlayer::update()
{
	Engine::BaseGameObject* player = gameObject->getScene()->getGameObjectWithTag("player");

	if (player == nullptr)
	{
		return;
	}

	gameObject->transform.position = glm::vec3(
		player->transform.position.x + _offset.x,
		gameObject->transform.position.y,
		player->transform.position.z + _offset.z
	);
}
