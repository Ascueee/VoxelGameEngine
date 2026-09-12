#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include "Rig.h"

struct TestComponent {
    std::string msg = "Hello";
};

struct OpenGLComponent {
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    GLsizei indexCount = 0;
};

struct MeshComponent {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    int rigRef = 0;
};


struct ColorComponent{
    glm::vec3 color = glm::vec3(1.0f);
};

struct TransformComponent{
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
};

struct RigComponent{
    Rig rig;
    std::vector<glm::mat4> finalBoneMatrices;
};

struct AnimatorComponent{
    float currentTime = 0.0f;
};

