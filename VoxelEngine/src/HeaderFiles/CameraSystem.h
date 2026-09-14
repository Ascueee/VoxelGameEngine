#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "StorageManager.h"

class CameraSystem{
    public:
        CameraSystem();
        static void ConstructCamera(Entity* ent, glm::vec3 offSet = glm::vec3(0,0,0));
};