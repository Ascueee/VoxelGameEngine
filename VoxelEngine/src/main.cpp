#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include "StorageManager.h"
#include "EngineModelLoader.h"
#include "EngineTextureLoader.h"
#include "Animator.h"
#include "RenderSystem.h"
#include "TransformSystem.h"
#include "AABBSystem.h"
#include "PhysicsSystem.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, float deltaTime);

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Voxel Game", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    EngineModelLoader::Load();
    EngineTextureLoader::Load();

    TransformSystem::Init(StorageManager::GetStorage());
    Animator::storage = StorageManager::GetStorage();
    RenderSystem::Init(StorageManager::GetStorage(), 
    new Shader("/Users/hayyan/Desktop/Repos/CPPEngine/VoxelEngine/src/Shaders/base.vert",
        "/Users/hayyan/Desktop/Repos/CPPEngine/VoxelEngine/src/Shaders/base.frag"));
    AABBSystem::Init(StorageManager::GetStorage());
    PhysicsSystem::Init(StorageManager::GetStorage());

    StorageManager::CreateBlankEntity();
    StorageManager::CreateCubeEntity();
    StorageManager::GetStorage()->colorStorage[1].color = glm::vec3(1,1,1);
    StorageManager::GetStorage()->materialStorage[1].diffuse = EngineTextureLoader::GetTexture("Brick");
    StorageManager::GetStorage()->transformStorage[1].scale = glm::vec3(10,1,10);
    PhysicsComponent boxPhys;
    boxPhys.isStatic = true;
    StorageManager::GetStorage()->physicsStorage.emplace(StorageManager::GetEntityById(1)->GetID(), boxPhys);

    StorageManager::CreateCubeEntity();
    StorageManager::GetStorage()->materialStorage[2].color = glm::vec3(1,1,0);
    StorageManager::GetStorage()->materialStorage[2].diffuse = EngineTextureLoader::GetTexture("grass");
    StorageManager::GetStorage()->transformStorage[2].position = glm::vec3(-2,1,0);
    StorageManager::GetStorage()->transformStorage[2].scale = glm::vec3(1,1,1);

    StorageManager::BuildModel(EngineModelLoader::GetModel("Ymca Dance"));
    StorageManager::GetStorage()->transformStorage[3].scale = glm::vec3(0.1f);
    StorageManager::GetStorage()->transformStorage[3].position = glm::vec3(0,5,0);
    StorageManager::GetStorage()->materialStorage[4].color = glm::vec3(1,1,1);
    StorageManager::GetStorage()->materialStorage[4].diffuse = EngineTextureLoader::GetTexture("Brick");
    PhysicsComponent modelPhys;
    StorageManager::GetStorage()->physicsStorage.emplace(StorageManager::GetEntityById(3)->GetID(), modelPhys);

    AABBSystem::ConstructAABB(StorageManager::GetEntityById(3), glm::vec3(1,1,1));
    AABBSystem::ConstructAABB(StorageManager::GetEntityById(1), glm::vec3(1,0.5,1));

    PhysicsSystem::Add(StorageManager::GetEntityById(3));
    PhysicsSystem::Add(StorageManager::GetEntityById(1));
    
    RenderSystem::Load(StorageManager::GetEntityById(1));
    RenderSystem::Load(StorageManager::GetEntityById(2));
    RenderSystem::Load(StorageManager::GetEntityById(4));

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        TransformSystem::Update(StorageManager::GetEntityById(1));
        TransformSystem::Update(StorageManager::GetEntityById(2));
        TransformSystem::Update(StorageManager::GetEntityById(3));
        Animator::RunAnimation(StorageManager::GetEntityById(3), "mixamo.com", deltaTime);
        PhysicsSystem::Run(deltaTime);
        RenderSystem::Draw(StorageManager::GetEntityById(1));
        RenderSystem::Draw(StorageManager::GetEntityById(2));
        RenderSystem::Draw(StorageManager::GetEntityById(4));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}