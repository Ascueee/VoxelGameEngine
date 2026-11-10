#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Package.hpp>
class CameraController : public Package{
    public:
        CameraController() = default;
        glm::mat4 GetView();
        glm::mat4 GetProjection();
        glm::vec3 position;
    private:
        glm::vec3 front = glm::vec3(0, 0, -1);
        glm::vec3 up = glm::vec3(0, 1, 0);

};