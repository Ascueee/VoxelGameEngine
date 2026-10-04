#include <Camera.hpp>

Camera::Camera(){}

Camera::Camera(glm::vec3 position){
    this->position = position;
}

void Camera::SetPosition(glm::vec3 position){
    this->position = position;
}

glm::vec3 Camera::GetPosition(){
    return position;
}

glm::vec3 Camera::GetFront(){
    return front;
}

glm::vec3 Camera::GetUp(){
    return up;
}

glm::mat4 Camera::GetView(){
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::GetProjection(){
    return glm::perspective(glm::radians(60.0f), 1280.0f / 720.0f,  0.1f, 1000.0f);
}