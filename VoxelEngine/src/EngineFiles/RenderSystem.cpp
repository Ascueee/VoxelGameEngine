#include "RenderSystem.h"

Storage* RenderSystem::storage;
Shader* RenderSystem::shader;

void RenderSystem::Init(Storage* newStorage, Shader* newShader){
    storage = newStorage;
    shader = newShader;
}

void RenderSystem::Load(Entity* ent){
    MeshComponent& meshComponent = storage->meshStorage[ent->GetID()];
    OpenGLComponent& openGLComponent = storage->openGLStorage[ent->GetID()]; 

    glGenVertexArrays(1, &openGLComponent.vao);
    glGenBuffers(1, &openGLComponent.vbo);
    glGenBuffers(1, &openGLComponent.ebo);

    glBindVertexArray(openGLComponent.vao);
    glBindBuffer(GL_ARRAY_BUFFER, openGLComponent.vbo);
    glBufferData(GL_ARRAY_BUFFER, meshComponent.vertices.size() * sizeof(float),meshComponent.vertices.data(),GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, openGLComponent.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,meshComponent.indices.size() * sizeof(unsigned int),meshComponent.indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 13 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 13 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 13 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

     glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 13 * sizeof(float), (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 

    openGLComponent.indexCount = static_cast<GLsizei>(meshComponent.indices.size());
}

void RenderSystem::Draw(Entity* ent){
    auto meshIt = storage->meshStorage.find(ent->GetID());
    auto openGLIt = storage->openGLStorage.find(ent->GetID());
    auto colorIt = storage->colorStorage.find(ent->GetID());
    auto materialIt = storage->materialStorage.find(ent->GetID());

    if (meshIt == storage->meshStorage.end() || openGLIt == storage->openGLStorage.end() || colorIt == storage->colorStorage.end()) {
        std::cout << "Couldnt Find Render Componenets" << std::endl;
        return;
    }

    // Debug-only camera — hardcoded here for now, will move to a real camera/scene system later
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 4.0f, 5.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),(float)1920 / (float)1080, 0.1f, 100.0f);
    glm::mat4 model = glm::mat4(1.0f);

    shader->Use();
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    auto rigIt = storage->rigStorage.find(meshIt->second.rigRef);

    MeshComponent MeshComponent = meshIt->second;
    OpenGLComponent openGLComponent = openGLIt->second;
    ColorComponent colorComponent = colorIt->second;
    MaterialComponent materialComponent = materialIt->second;
    TransformComponent transformComponent = storage->transformStorage[ent->GetID()];

    glBindTexture(GL_TEXTURE_2D, materialComponent.diffuse->GetHandle());
    shader->setInt("diffuseTexture", 0);
    shader->setMat4("model", transformComponent.model);
    shader->setVec3("color", materialComponent.color);

    if (rigIt != storage->rigStorage.end() && !rigIt->second.finalBoneMatrices.empty()) {
        shader->setMat4Array("finalBonesMatrices", rigIt->second.finalBoneMatrices);
    }

    glBindVertexArray(openGLComponent.vao);
    glDrawElements(GL_TRIANGLES, openGLComponent.indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}