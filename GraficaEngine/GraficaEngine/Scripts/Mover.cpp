#include "Mover.h"

#include "../Core/Time.h"
#include "../Core/GameObject.h"

Mover::Mover(float speed = 10.f): _speed(speed)
{}

Mover *Mover::clone() const
{
	return new Mover(_speed);
}

void Mover::update()
{
	Engine::Transform& transform = gameObject->transform;
	transform.position += transform.getForward() * _speed * Engine::Time::getDeltaTime();
}