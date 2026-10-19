#include <DebugRenderer.hpp>

Shader DebugRenderer::shader;
Camera* DebugRenderer::camera;
DebugRenderer::DebugRenderer(){}

void DebugRenderer::Load(Camera* cam){
    shader = Shader("/Users/hayyan/Desktop/Repos/CPPEngine/Rewrite/src/Assets/Shaders/Debug.vert",
        "/Users/hayyan/Desktop/Repos/CPPEngine/Rewrite/src/Assets/Shaders/Debug.frag");
    
    camera = cam;
}

//Load a cube mesh to represent an AABB
void DebugRenderer::LoadAABB(Mesh& mesh){
    mesh.GetVertices() = {
        // Front face
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        // Back face
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        // Left face
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,

        // Right face
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,

        // Bottom face
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        // Top face
        -0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f
    };

    mesh.GetIndices() = {
        // Front face edges
        0, 1,  1, 2,  2, 3,  3, 0,

        // Back face edges
        4, 5,  5, 6,  6, 7,  7, 4,

        // Edges connecting front to back
        0, 4,  1, 5,  2, 6,  3, 7
    };

    glGenBuffers(1, &mesh.GetVBO());
    glGenVertexArrays(1, &mesh.GetVAO());
    glGenBuffers(1, &mesh.GetEBO());

    glBindVertexArray(mesh.GetVAO());

    glBindBuffer(GL_ARRAY_BUFFER, mesh.GetVBO());
    glBufferData(GL_ARRAY_BUFFER, mesh.GetVertices().size() * sizeof(float), mesh.GetVertices().data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.GetEBO());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.GetIndices().size() * sizeof(unsigned int), mesh.GetIndices().data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
}


void DebugRenderer::DrawAABB(Mesh& mesh, AABB* aabb){
    glBindVertexArray(mesh.GetVAO());
    shader.Use();

    //create model based on AABB
    glm::vec3 center = (aabb->GetMin() + aabb->GetMax()) * 0.5f;
    glm::vec3 size = aabb->GetMax() - aabb->GetMin();

    glm::mat4 model = glm::mat4(1.0f);

    float blockSize = 4;
    model = glm::translate(model, center * blockSize);
    model = glm::scale(model, size * blockSize);
    
    shader.setMat4("view", camera->GetView());
    shader.setMat4("projection", camera->GetProjection());
    shader.setMat4("model", model);

    shader.setVec3("color", glm::vec3(1,0,1));

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_LINES, mesh.GetIndices().size(), GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

