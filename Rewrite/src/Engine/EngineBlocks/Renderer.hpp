#pragma once
#include <Mesh.hpp>
#include <Shader.h>
#include <glad/glad.h>
#include <CameraController.hpp>
#include <TextureAtlasStorage.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <queue>
#include <Transform.hpp>

struct GBuffer{
    unsigned int handle;
    unsigned int gPosition;
    unsigned int gDiffuse; 
    unsigned int gNormal;
    unsigned int rboDepth;
};

struct RenderQuad{
    std::vector<float> quadVertices = {
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,   
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,   
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f,   

        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,   
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,   
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f    
    };
    unsigned int renderQuadVAO;
    unsigned int renderQuadVBO;
};

struct DirectionalLight{
    glm::vec3 position;

    glm::vec3 ambient;
    glm::vec3 color;
    glm::vec3 specular;
};

struct PointLight{
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;
	
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

class Renderer{
    public:
        Renderer();
        static void Load();
        static void LoadMesh(Mesh& mesh);
        static void LoadGBuffer();
        static void SetCamera(CameraController* camera);
        static void InitializeGeometryPass();
        static void InitializeForwardPass(float screenWidth, float screenHeight);
        static void DeferredPass(float screenWidth, float screenHeight);
        static void GeometryPassChunk(Mesh& mesh, glm::vec3 position);
        static void ForwardPassChunk(Mesh& mesh, glm::vec3 position);
        static void GeometryPassEntity(Mesh& mesh, Transform* transform);
        static void ForwardPassEntity(Mesh& mesh, Transform* transform);
        static void InitializeIds();
        static void Delete(Mesh& mesh);
    private:
        static Shader forwardPass;
        static Shader geometryPass;
        static Shader renderPass;
        static CameraController* camera;
        static GBuffer gBuffer;
        static RenderQuad renderQuad;
        static std::queue<ushort> rendererIds;
};