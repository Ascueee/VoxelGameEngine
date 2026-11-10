#pragma once
#include <glm/glm.hpp>
#include <Package.hpp>
class Transform : public Package{
    public:
        Transform() = default;
        glm::vec3 position = glm::vec3(0.0f);
};