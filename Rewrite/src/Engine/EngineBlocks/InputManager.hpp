#pragma once
#include <iostream>
#include <unordered_map>
#include <GLFW/glfw3.h>

enum Keys{
    W,
    A,
    S,
    D
};


class InputManager{
    public:
        InputManager();
        static void Load(GLFWwindow* currentWindow);
        static void MousePosition(GLFWwindow* window, double xposIn, double yposIn);
        static bool IsKeyPressed(Keys key);
        static float GetMouseX();
        static float GetMouseY();
    private:
        static std::unordered_map<Keys, int> keys;
        static float mouseX;
        static float mouseY;
        static float lastMouseX;
        static float lastMouseY;
        static bool firstRun;
        static GLFWwindow* window;
};