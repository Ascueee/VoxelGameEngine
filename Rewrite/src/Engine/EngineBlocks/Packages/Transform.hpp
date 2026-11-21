#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Package.hpp>

class Transform : public Package{
    public:
        Transform() = default;
        glm::vec3 position = glm::vec3(0.0f);
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 scale = glm::vec3(1.0f);
};