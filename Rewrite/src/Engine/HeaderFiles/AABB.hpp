#pragma once
#include <glm/glm.hpp>

class AABB{
    public:
        AABB();
        AABB(glm::vec3 position, glm::vec3 size);
        glm::vec3 GetMin();
        glm::vec3 GetMax();
        glm::vec3 GetSize();
    private:
        glm::vec3 size = glm::vec3(1,1,1);
        glm::vec3 min;
        glm::vec3 max;
};
