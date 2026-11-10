#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera{
    public:
        Camera();
        Camera(glm::vec3 position);
        void SetPosition(glm::vec3 pos);
        glm::mat4 GetView();
        glm::mat4 GetProjection();
        glm::vec3 GetPosition();
        glm::vec3 GetFront();
        glm::vec3 GetUp();
        
    private:
        glm::vec3 position;
        glm::vec3 up = glm::vec3(0,1,0);
        glm::vec3 front = glm::vec3(0,0,-1);

};