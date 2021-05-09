#include "FirstPersonCameraController.h"

#include "../Core/Input.h"
#include "../Core/Time.h"
#include "../Core/Camera.h"
#include "../Core/Scene.h"

FirstPersonCameraController::FirstPersonCameraController(glm::vec3 relativePosition) : _relativePosition(relativePosition)
{
}

void FirstPersonCameraController::update()
{
    Engine::Transform &transform = gameObject->transform;

    Engine::BaseGameObject *player = gameObject->getScene()->getGameObjectWithTag("player");

    if (player == nullptr)
    {
        return;
    }

    Engine::Transform &playerTransform = player->transform;

    transform.position = playerTransform.position;
    transform.position += playerTransform.getForward() * _relativePosition.x;
    transform.position += playerTransform.getRight() * _relativePosition.z;
    transform.position += playerTransform.getUp() * _relativePosition.y;

    Engine::Input &input = Engine::Input::getInstance();

    int movementX, movementY;
    std::tie(movementX, movementY) = input.getMouseMovement();

    Engine::Camera *camera = static_cast<Engine::Camera *>(gameObject);

    camera->processMouseMovement(float(movementX) * Engine::Time::getDeltaTime(), float(movementY) * Engine::Time::getDeltaTime());

    glm::vec3 cameraForward = transform.getForward();
    glm::vec3 lookDirection = glm::vec3(transform.getForward().x, 0.f, transform.getForward().z);
    playerTransform.lookAt(lookDirection);
}
