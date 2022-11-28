#include "ThirdPersonCameraController.h"

#include "../Core/Transform.h"
#include "../Core/Input.h"
#include "../Core/Camera.h"
#include "../Core/Time.h"
#include "../Core/GameObject.h"

#include "../Utils/DebugLog.h"

ThirdPersonCameraController::ThirdPersonCameraController(float distance, glm::vec3 offset) : _distance(distance), _offset(offset), _xyMovement(glm::vec3(0.f))
{
}

ThirdPersonCameraController *ThirdPersonCameraController::clone() const
{
    return new ThirdPersonCameraController(_distance, _offset);
}

void ThirdPersonCameraController::update()
{
    auto& transform = gameObject->transform;
    auto* player = gameObject->getScene()->getGameObjectWithTag("player");

    if (player == nullptr) {
        return;
    }

    auto& playerTransform = player->transform;
    auto& input = Engine::Input::getInstance();

    float threshold = 0.1f;

    int mouseX, mouseY;
    std::tie(mouseX, mouseY) = input.getMouseMovement();
    _xyMovement.x += float(mouseX) * Engine::Time::getDeltaTime() * _sensitivityX;
    _xyMovement.y += float(mouseY) * Engine::Time::getDeltaTime() * _sensitivityY;

    _xyMovement.y = glm::clamp(_xyMovement.y, 0.f + threshold, glm::pi<float>() - threshold);
    // Get cartesian from spherical
    float x = glm::sin(_xyMovement.y) * glm::cos(_xyMovement.x);
    float y = glm::cos(_xyMovement.y);
    float z = glm::sin(_xyMovement.y) * glm::sin(_xyMovement.x);
    glm::vec3 coordinates(x, y, z);

    glm::vec3 target = playerTransform.position;
    transform.position = target + coordinates * _distance;

    transform.lookAt(target - transform.position + _offset);

    glm::vec3 cameraForward = transform.getForward();
    glm::vec3 lookDirection = glm::vec3(transform.getForward().x, 0.f, transform.getForward().z);
    playerTransform.lookAt(lookDirection);

    auto quaternionLookAt = glm::quatLookAt(lookDirection, transform.getUp());

    auto* rigidBody = ((Engine::GameObject*)player)->getRigidBody();
    auto& rigidBodyTransform = rigidBody->getWorldTransform();
    auto quaternion = btQuaternion(quaternionLookAt.x, quaternionLookAt.y, quaternionLookAt.z, quaternionLookAt.w);
    rigidBodyTransform.setRotation(quaternion);
}
