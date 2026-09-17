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
#include "CameraSystem.h"
#include "PlayerMovementSystem.h"
#include "ChunkSystem.h"
#include "WorldGenerator.h"
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

    Entity* blankEnt = StorageManager::CreateBlankEntity();

    Entity* groundCube = StorageManager::CreateCubeEntity();
    StorageManager::GetStorage()->materialStorage[groundCube->GetID()].diffuse = EngineTextureLoader::GetTexture("Brick");
    StorageManager::GetStorage()->transformStorage[groundCube->GetID()].scale = glm::vec3(100,1,100);
    PhysicsComponent boxPhys;
    boxPhys.isStatic = true;
    StorageManager::GetStorage()->physicsStorage.emplace(StorageManager::GetEntityById(1)->GetID(), boxPhys);

    Entity* testCube = StorageManager::CreateCubeEntity();
    StorageManager::GetStorage()->materialStorage[testCube->GetID()].color = glm::vec3(1,0,0);
    StorageManager::GetStorage()->materialStorage[testCube->GetID()].diffuse = EngineTextureLoader::GetTexture("Grass");
    StorageManager::GetStorage()->transformStorage[testCube->GetID()].position = glm::vec3(-2,1,0);
    StorageManager::GetStorage()->transformStorage[testCube->GetID()].scale = glm::vec3(1,1,1);

    Entity* walkingRootEntity = StorageManager::BuildModel(EngineModelLoader::GetModel("Walking"));
    StorageManager::GetStorage()->transformStorage[walkingRootEntity->GetID()].scale = glm::vec3(0.1f);
    StorageManager::GetStorage()->transformStorage[walkingRootEntity->GetID()].position = glm::vec3(50,40,0);
    StorageManager::GetStorage()->materialStorage[walkingRootEntity->GetID() + 1].color = glm::vec3(1,1,1);
    StorageManager::GetStorage()->materialStorage[walkingRootEntity->GetID() + 1].diffuse = EngineTextureLoader::GetTexture("Grass");
    StorageManager::GetStorage()->AABBStorage[walkingRootEntity->GetID()].offSet = glm::vec3(0,2,0);
    PhysicsComponent modelPhys;

    StorageManager::GetStorage()->physicsStorage.emplace(StorageManager::GetEntityById(3)->GetID(), modelPhys);
    AABBSystem::ConstructAABB(StorageManager::GetEntityById(walkingRootEntity->GetID()), glm::vec3(10,2,10));
    AABBSystem::ConstructAABB(StorageManager::GetEntityById(groundCube->GetID()), glm::vec3(2,0.5,2));

    Entity* cameraEnt = StorageManager::CreateCameraEntity();
    StorageManager::GetStorage()->entityStorage[cameraEnt->GetID()].SetParentID(walkingRootEntity->GetID());
    StorageManager::GetStorage()->entityStorage[walkingRootEntity->GetID()].SetChild(cameraEnt->GetID());
    RenderSystem::SetRenderCamera(StorageManager::GetEntityById(cameraEnt->GetID()));

    //Generates the world
    WorldGenerator::GenerateWorld(cameraEnt, new Shader("/Users/hayyan/Desktop/Repos/CPPEngine/VoxelEngine/src/Shaders/chunk.vert",
        "/Users/hayyan/Desktop/Repos/CPPEngine/VoxelEngine/src/Shaders/chunk.frag"));

    //Adds entities to physics system
    PhysicsSystem::Add(StorageManager::GetEntityById(walkingRootEntity->GetID()));
    PhysicsSystem::Add(StorageManager::GetEntityById(groundCube->GetID()));

    //Loads entities into the renderer
    RenderSystem::Load(StorageManager::GetEntityById(groundCube->GetID()));
    RenderSystem::Load(StorageManager::GetEntityById(testCube->GetID()));
    RenderSystem::Load(StorageManager::GetEntityById(walkingRootEntity->GetID() + 1));

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        PhysicsSystem::Run(deltaTime);
        TransformSystem::Update(StorageManager::GetEntityById(groundCube->GetID()));
        TransformSystem::Update(StorageManager::GetEntityById(testCube->GetID()));
        TransformSystem::Update(StorageManager::GetEntityById(walkingRootEntity->GetID()));
        TransformSystem::Update(StorageManager::GetEntityById(cameraEnt->GetID()));
        CameraSystem::ConstructCamera(StorageManager::GetEntityById(cameraEnt->GetID()), glm::vec3(0,2.0f,-10));
        Animator::RunAnimation(StorageManager::GetEntityById(walkingRootEntity->GetID()), "mixamo.com", deltaTime);


        WorldGenerator::RenderWorld(StorageManager::GetEntityById(walkingRootEntity->GetID()));
        RenderSystem::Draw(StorageManager::GetEntityById(groundCube->GetID()));
        RenderSystem::Draw(StorageManager::GetEntityById(testCube->GetID()));
        RenderSystem::Draw(StorageManager::GetEntityById(walkingRootEntity->GetID() + 1));
        processInput(window, deltaTime);

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
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        PlayerMovementSystem::Move(StorageManager::GetEntityById(3), Direction::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        PlayerMovementSystem::Move(StorageManager::GetEntityById(3), Direction::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        PlayerMovementSystem::Move(StorageManager::GetEntityById(3), Direction::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        PlayerMovementSystem::Move(StorageManager::GetEntityById(3), Direction::LEFT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}