#pragma once
#include <glm/glm.hpp>
#include <Mesh.hpp>

class AABB{
    public:
        AABB();
        AABB(glm::vec3 position, glm::vec3 size);
        bool IsColliding(AABB* other);
        glm::vec3 GetMin() const;
        glm::vec3 GetMax();
        glm::vec3 GetSize();
        Mesh& GetMesh();
    private:
        glm::vec3 size = glm::vec3(1,1,1);
        glm::vec3 min;
        glm::vec3 max;
        Mesh debugMesh;
};
