#include "EngineDebug.h"

Shader* EngineDebug::shader;
int EngineDebug::renderCamera;
EngineDebug::EngineDebug(){}

void EngineDebug::Load(Entity* ent){
    DebugGLComponent& debugGL = StorageManager::GetStorage()->debugGLStorage[ent->GetID()];
    AABBDebugComponent& aabbDebug = StorageManager::GetStorage()->AABBDebugStorage[ent->GetID()];


    aabbDebug.vertices = {
        // x     y     z
        -0.5f, -0.5f, -0.5f, // 0
        0.5f, -0.5f, -0.5f, // 1
        0.5f,  0.5f, -0.5f, // 2
        -0.5f,  0.5f, -0.5f, // 3

        -0.5f, -0.5f,  0.5f, // 4
        0.5f, -0.5f,  0.5f, // 5
        0.5f,  0.5f,  0.5f, // 6
        -0.5f,  0.5f,  0.5f  // 7
    };

    aabbDebug.indices = {
        // Back
        0, 1, 2,
        2, 3, 0,

        // Front
        4, 5, 6,
        6, 7, 4,

        // Left
        0, 3, 7,
        7, 4, 0,

        // Right
        1, 5, 6,
        6, 2, 1,

        // Bottom
        0, 4, 5,
        5, 1, 0,

        // Top
        3, 2, 6,
        6, 7, 3
    };

    glGenVertexArrays(1, &debugGL.vao);
    glGenBuffers(1, &debugGL.vbo);
    glGenBuffers(1, &debugGL.ebo);

    glBindVertexArray(debugGL.vao);
    glBindBuffer(GL_ARRAY_BUFFER, debugGL.vbo);
    glBufferData(GL_ARRAY_BUFFER, aabbDebug.vertices.size() * sizeof(float), aabbDebug.vertices.data(),GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, debugGL.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, aabbDebug.indices.size() * sizeof(unsigned int), aabbDebug.indices.data(), GL_STATIC_DRAW);

    //Stores position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 

    debugGL.indexCount = static_cast<GLsizei>(aabbDebug.indices.size());
}


void EngineDebug::Draw(Entity* ent){
    DebugGLComponent& debugGL = StorageManager::GetStorage()->debugGLStorage[ent->GetID()];
    AABBDebugComponent& aabbDebug = StorageManager::GetStorage()->AABBDebugStorage[ent->GetID()];
    CameraComponent& cameraComponent = StorageManager::GetStorage()->cameraStorage[renderCamera];
    AABBComponent& aabbComponent = StorageManager::GetStorage()->AABBStorage[ent->GetID()];
    TransformComponent& transform = StorageManager::GetStorage()->transformStorage[ent->GetID()];

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, aabbComponent.center);
    model = glm::scale(model, aabbComponent.size);


    shader->Use();
    shader->setMat4("view", cameraComponent.view);
    shader->setMat4("projection", cameraComponent.projection);
    shader->setMat4("model", model);
    shader->setVec3("color", glm::vec3(1,0,1));

    glBindVertexArray(debugGL.vao);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, debugGL.indexCount, GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(0);

}

void EngineDebug::AddShader(Shader* newShader){
    shader = newShader;
}


void EngineDebug::AddCamera(Entity* ent){
    renderCamera = ent->GetID();
}
