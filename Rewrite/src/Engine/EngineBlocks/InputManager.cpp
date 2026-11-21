#include <InputManager.hpp>

std::unordered_map<Keys, int> InputManager::keys;
GLFWwindow* InputManager::window;


float InputManager::mouseX;
float InputManager::mouseY;
float InputManager::lastMouseX;
float InputManager::lastMouseY;
bool InputManager::firstRun = true;
InputManager::InputManager(){}

void InputManager::Load(GLFWwindow* currentWindow){
    window = currentWindow;
    keys = {
        {W, GLFW_KEY_W},
        {A, GLFW_KEY_A},
        {S, GLFW_KEY_S},
        {D, GLFW_KEY_D}
    };
}

void InputManager::MousePosition(GLFWwindow* window, double xposIn, double yposIn){
    float currentMouseX = static_cast<float>(xposIn);
    float currentMouseY = static_cast<float>(yposIn);

    if(firstRun){
        lastMouseX = currentMouseX;
        lastMouseY = currentMouseY;
        firstRun = false;
    }

    mouseX = currentMouseX - lastMouseX;
    mouseY = lastMouseY - currentMouseY;
    lastMouseX = currentMouseX;
    lastMouseY = currentMouseY;

    float sensitivity = 0.1f;
    mouseX *= sensitivity;
    mouseY *= sensitivity;
}

bool InputManager::IsKeyPressed(Keys key){
    if (glfwGetKey(window, keys[key]) == GLFW_PRESS){
        return true;
    }
    else{
        return false;
    }
}

float InputManager::GetMouseX(){
    return mouseX;
}

float InputManager::GetMouseY(){
    return mouseY;
}