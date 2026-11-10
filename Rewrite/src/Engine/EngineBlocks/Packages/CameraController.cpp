#include <CameraController.hpp>


glm::mat4 CameraController::GetView(){
    return glm::lookAt(position, position + front, up);
}

glm::mat4 CameraController::GetProjection(){
    return glm::perspective(glm::radians(60.0f), 1280.0f / 720.0f,  0.1f, 2000.0f);
}