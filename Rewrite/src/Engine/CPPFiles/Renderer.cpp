#include <Renderer.hpp>

Shader Renderer::shader;
Shader Renderer::geometryPass;
Shader Renderer::renderPass;
Camera* Renderer::camera;
GBuffer Renderer::gBuffer;
RenderQuad Renderer::renderQuad;
Renderer::Renderer(){}

void Renderer::Load(Camera* cam){
    shader = Shader("/Users/hayyan/Desktop/Repos/CPPEngine/Rewrite/src/Assets/Shaders/base.vert",
        "/Users/hayyan/Desktop/Repos/CPPEngine/Rewrite/src/Assets/Shaders/base.frag");
    geometryPass = Shader("/Users/hayyan/Desktop/Repos/CPPEngine/Rewrite/src/Assets/Shaders/GeometryPass.vert",
        "/Users/hayyan/Desktop/Repos/CPPEngine/Rewrite/src/Assets/Shaders/GeometryPass.frag");
    renderPass = Shader("/Users/hayyan/Desktop/Repos/CPPEngine/Rewrite/src/Assets/Shaders/RenderPass.vert",
        "/Users/hayyan/Desktop/Repos/CPPEngine/Rewrite/src/Assets/Shaders/RenderPass.frag");
    
    camera = cam;
}

//gets a mesh and uploads it to the gpu for rendering
void Renderer::LoadMesh(Mesh& mesh){
    glGenBuffers(1, &mesh.GetVBO());
    glGenVertexArrays(1, &mesh.GetVAO());
    glGenBuffers(1, &mesh.GetEBO());

    glBindVertexArray(mesh.GetVAO());

    glBindBuffer(GL_ARRAY_BUFFER, mesh.GetVBO());
    glBufferData(GL_ARRAY_BUFFER, mesh.GetVertices().size() * sizeof(float), mesh.GetVertices().data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.GetEBO());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.GetIndices().size() * sizeof(unsigned int), mesh.GetIndices().data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

void Renderer::LoadGBuffer(){
    glGenFramebuffers(1, &gBuffer.handle);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer.handle);

    //generates position texture for rendering
    glGenTextures(1, & gBuffer.gPosition);
    glBindTexture(GL_TEXTURE_2D, gBuffer.gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280, 720, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gBuffer.gPosition, 0);

    //generates color texture for rendering
    glGenTextures(1, &gBuffer.gDiffuse);
    glBindTexture(GL_TEXTURE_2D, gBuffer.gDiffuse);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280, 720, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gBuffer.gDiffuse, 0);

    glGenTextures(1, &gBuffer.gNormal);
    glBindTexture(GL_TEXTURE_2D, gBuffer.gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280, 720, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gBuffer.gNormal, 0);

    //attaches for buffers for drawing
    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);

    //Generates depth buffer
    glGenRenderbuffers(1, &gBuffer.rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, gBuffer.rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1280, 720);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gBuffer.rboDepth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    //initialize the render quad for the render pass
    glGenBuffers(1, &renderQuad.renderQuadVBO);
    glGenVertexArrays(1, &renderQuad.renderQuadVAO);

    glBindVertexArray(renderQuad.renderQuadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, renderQuad.renderQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, renderQuad.quadVertices.size() * sizeof(float), renderQuad.quadVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

void Renderer::InitializeGeometryPass(){
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer.handle);
    glViewport(0, 0, 1280, 720);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::GeometryPass(Mesh& mesh, glm::vec3 position){
    //Geometry pass
    glBindVertexArray(mesh.GetVAO());
    geometryPass.Use();

    float blockSize = 4;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position * blockSize);
    model = glm::scale(model, glm::vec3(blockSize));

    //set texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureAtlasStorage::GetTexture("BaseAtlas")->GetHandle());
    geometryPass.setInt("diffuse", 0);

    geometryPass.setMat4("view", camera->GetView());
    geometryPass.setMat4("projection", camera->GetProjection());
    geometryPass.setMat4("model", model);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, mesh.GetIndices().size(), GL_UNSIGNED_INT, 0);
}

void Renderer::RenderPass(float screenWidth, float screenHeight){
    glBindVertexArray(renderQuad.renderQuadVAO);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, screenWidth, screenHeight);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    renderPass.Use();

    //binds position
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gBuffer.gPosition);
    renderPass.setInt("gPosition", 0);

    // Bind the diffuse texture stored during the geometry pass
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gBuffer.gDiffuse);
    renderPass.setInt("gDiffuse", 1);

    //binds normal
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gBuffer.gNormal);
    renderPass.setInt("gNormal", 2);

    renderPass.setVec3("viewPos", camera->GetPosition());
    
    //set lighting shader vars
    renderPass.setVec3("dirLight.direction", glm::vec3(-0.5f, -1.0f, -0.5f));
    renderPass.setVec3("dirLight.ambient", glm::vec3(0.7f));
    renderPass.setVec3("dirLight.diffuse", 1.0f, 1.0f, 1.0f);
    renderPass.setVec3("dirLight.specular", glm::vec3(0.8f));

    renderPass.setVec3("pointLights[0].position", camera->GetPosition());
    renderPass.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    renderPass.setVec3("pointLights[0].diffuse", 1.0f, 0.0f, 0.0f);
    renderPass.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    renderPass.setFloat("pointLights[0].constant", 1.0f);
    renderPass.setFloat("pointLights[0].linear", 0.0f);
    renderPass.setFloat("pointLights[0].quadratic", 0.0f);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
}

void Renderer::Delete(Mesh& mesh){
    glDeleteBuffers(1, &mesh.GetVBO());
    glDeleteVertexArrays(1, &mesh.GetVAO());
    glDeleteBuffers(1, &mesh.GetEBO());
}


