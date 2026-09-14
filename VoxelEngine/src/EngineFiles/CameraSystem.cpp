#include "CameraSystem.h"

CameraSystem::CameraSystem(){};

void CameraSystem::ConstructCamera(Entity* ent, glm::vec3 offSet){
    CameraComponent& cam = StorageManager::GetStorage()->cameraStorage[ent->GetID()];
    TransformComponent& trans = StorageManager::GetStorage()->transformStorage[ent->GetID()];
     glm::vec3 worldPos = glm::vec3(trans.model[3]) + offSet;
    cam.view = glm::lookAt(worldPos, worldPos + cam.front, cam.up);
    cam.projection = glm::perspective(glm::radians(45.0f),(float)1920 / (float)1080, 0.1f, 100.0f);
}