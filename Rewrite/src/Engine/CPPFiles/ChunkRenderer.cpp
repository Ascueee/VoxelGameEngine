#include "ChunkRenderer.hpp"

Shader ChunkRenderer::shader;
Camera* ChunkRenderer::camera;
ChunkRenderer::ChunkRenderer(){}

void ChunkRenderer::Load(Camera* cam){
    shader = Shader("/Users/hayyan/Desktop/Repos/CPPEngine/Rewrite/src/Assets/Shaders/base.vert",
        "/Users/hayyan/Desktop/Repos/CPPEngine/Rewrite/src/Assets/Shaders/base.frag");
    camera = cam;
}

void ChunkRenderer::LoadMesh(Chunk& chunk, glm::vec3 pos){
    //std::cout << "[ChunkRenderer] Loading chunk mesh \n";
    Mesh& chunkMesh = chunk.GetMesh();

    glGenBuffers(1, &chunkMesh.GetVBO());
    glGenVertexArrays(1, &chunkMesh.GetVAO());
    glGenBuffers(1, &chunkMesh.GetEBO());

    glBindVertexArray(chunkMesh.GetVAO());

    glBindBuffer(GL_ARRAY_BUFFER, chunkMesh.GetVBO());
    glBufferData(GL_ARRAY_BUFFER, chunkMesh.GetVertices().size() * sizeof(float), chunkMesh.GetVertices().data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunkMesh.GetEBO());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunkMesh.GetIndices().size() * sizeof(unsigned int), chunkMesh.GetIndices().data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

void ChunkRenderer::Draw(Chunk& chunk, glm::vec3 pos){
    shader.Use();
    glBindVertexArray(chunk.GetMesh().GetVAO());

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, chunk.GetPosition());

    //set texture
    glBindTexture(GL_TEXTURE_2D, TextureAtlasStorage::GetTexture("BaseAtlas")->GetHandle());
    shader.setInt("diffuse", 0);

    //set shader model, view, and projection
    shader.setMat4("view", camera->GetView());
    shader.setMat4("projection", camera->GetProjection());
    shader.setMat4("model", model);

    //set shader color
    shader.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, chunk.GetMesh().GetIndices().size(), GL_UNSIGNED_INT, 0);
}


//using the chunk gets its mesh and delete the vao, vbo, and ebo
void ChunkRenderer::Delete(Chunk& chunk){
    Mesh chunkMesh = chunk.GetMesh();
    
    glDeleteBuffers(1, &chunkMesh.GetVBO());
    glDeleteVertexArrays(1, &chunkMesh.GetVAO());
    glDeleteBuffers(1, &chunkMesh.GetEBO());
}