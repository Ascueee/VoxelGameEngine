#include "ChunkRenderer.h"

Shader* ChunkRenderer::shader;
int ChunkRenderer::renderCamera;
ChunkRenderer::ChunkRenderer(){}

void ChunkRenderer::Load(Entity* ent){
    MeshComponent& meshComponent = StorageManager::GetStorage()->meshStorage[ent->GetID()];
    OpenGLComponent& openGLComponent = StorageManager::GetStorage()->openGLStorage[ent->GetID()]; 

    glGenVertexArrays(1, &openGLComponent.vao);
    glGenBuffers(1, &openGLComponent.vbo);
    glGenBuffers(1, &openGLComponent.ebo);

    glBindVertexArray(openGLComponent.vao);
    glBindBuffer(GL_ARRAY_BUFFER, openGLComponent.vbo);
    glBufferData(GL_ARRAY_BUFFER, meshComponent.mesh.vertices.size() * sizeof(float),meshComponent.mesh.vertices.data(),GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, openGLComponent.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshComponent.mesh.indices.size() * sizeof(unsigned int),meshComponent.mesh.indices.data(), GL_STATIC_DRAW);

    //Stores position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Stores UVS
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 

    openGLComponent.indexCount = static_cast<GLsizei>(meshComponent.mesh.indices.size());
}

void ChunkRenderer::Draw(Entity* ent){
    MeshComponent MeshComponent = StorageManager::GetStorage()->meshStorage[ent->GetID()];
    OpenGLComponent openGLComponent = StorageManager::GetStorage()->openGLStorage[ent->GetID()];
    MaterialComponent materialComponent = StorageManager::GetStorage()->materialStorage[ent->GetID()];
    TransformComponent transformComponent = StorageManager::GetStorage()->transformStorage[ent->GetID()];
    CameraComponent cameraIt = StorageManager::GetStorage()->cameraStorage[renderCamera];
    
    shader->Use();
    shader->setMat4("view", cameraIt.view);
    shader->setMat4("projection", cameraIt.projection);

    glBindTexture(GL_TEXTURE_2D, materialComponent.diffuse->GetHandle());
    shader->setInt("diffuseTexture", 0);
    shader->setMat4("model", transformComponent.model);
    shader->setVec3("color", materialComponent.color);


    glBindVertexArray(openGLComponent.vao);
    glDrawElements(GL_TRIANGLES, openGLComponent.indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void ChunkRenderer::AddShader(Shader* newShader){
    shader = newShader;
}


void ChunkRenderer::AddCamera(Entity* ent){
    renderCamera = ent->GetID();
}

