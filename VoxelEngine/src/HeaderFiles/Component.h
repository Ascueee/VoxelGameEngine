#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include "Rig.h"
#include "Texture.h"
#include "Block.h"
#include "Mesh.h"

struct TransformComponent{
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 front = glm::vec3(0,0,1);
    glm::vec3 up = glm::vec3(0,1,0);
};

struct OpenGLComponent {
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    GLsizei indexCount = 0;
};

struct MeshComponent {
    Mesh mesh;
    int rigRef = 0;
};

struct MaterialComponent{
    glm::vec3 color = glm::vec3(1,1,1);
    Texture* diffuse = nullptr;
};

struct RigComponent{
    Rig rig;
    std::vector<glm::mat4> finalBoneMatrices;
};

struct AnimatorComponent{
    float currentTime = 0.0f;
};

struct PhysicsComponent{
    bool isStatic = false;
};

struct PlayerMovement{
    float speed = 5.0f;
};

struct AABBComponent{
    glm::vec3 center = glm::vec3(0,0,0);
    glm::vec3 offSet = glm::vec3(0,0,0);
    glm::vec3 size = glm::vec3(0,0,0);
    glm::vec3 halfExtents = glm::vec3(0,0,0);
    glm::vec3 min = glm::vec3(0,0,0);
    glm::vec3 max = glm::vec3(0,0,0);
};

struct CameraComponent{
    glm::mat4 view;
    glm::mat4 projection;
};

struct ChunkComponent{
    int width = 16;
    int height = 355;
    Block blocks[16][355][16];
    int chunkNeighbours[4] = { -1, -1, -1, -1}; //front, back, left right saves the chunks neighbours ids
};

 

