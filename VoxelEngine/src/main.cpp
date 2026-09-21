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
#include "EngineDebug.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Entity* ent, float deltaTime);

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
    EngineDebug::AddShader(new Shader("/Users/hayyan/Desktop/Repos/CPPEngine/VoxelEngine/src/Shaders/debug.vert",
        "/Users/hayyan/Desktop/Repos/CPPEngine/VoxelEngine/src/Shaders/debug.frag"));
        
    AABBSystem::Init(StorageManager::GetStorage());
    PhysicsSystem::Init(StorageManager::GetStorage());

    Entity* blankEnt = StorageManager::CreateBlankEntity();

    Entity* testCube = StorageManager::CreateCubeEntity();
    StorageManager::GetStorage()->materialStorage[testCube->GetID()].color = glm::vec3(1,1,1);
    StorageManager::GetStorage()->materialStorage[testCube->GetID()].diffuse = EngineTextureLoader::GetTexture("Grass");
    StorageManager::GetStorage()->transformStorage[testCube->GetID()].position = glm::vec3(100,15,100);
    StorageManager::GetStorage()->transformStorage[testCube->GetID()].scale = glm::vec3(1,1,1);
    PhysicsComponent testBoxPhys;
    DebugGLComponent debugTestGL;
    StorageManager::GetStorage()->physicsStorage.emplace(testCube->GetID(), testBoxPhys);
    StorageManager::GetStorage()->debugGLStorage.emplace(testCube->GetID(), debugTestGL);

    Entity* walkingRootEntity = StorageManager::BuildModel(EngineModelLoader::GetModel("Walking"));
    StorageManager::GetStorage()->transformStorage[walkingRootEntity->GetID()].scale = glm::vec3(0.01f);
    StorageManager::GetStorage()->transformStorage[walkingRootEntity->GetID()].position = glm::vec3(100,15,100);
    StorageManager::GetStorage()->materialStorage[walkingRootEntity->GetID() + 1].color = glm::vec3(1,1,1);
    StorageManager::GetStorage()->materialStorage[walkingRootEntity->GetID() + 1].diffuse = EngineTextureLoader::GetTexture("Grass");
    PhysicsComponent modelPhys;
    DebugGLComponent debugGLRoot;
    StorageManager::GetStorage()->physicsStorage.emplace(walkingRootEntity->GetID(), modelPhys);
    StorageManager::GetStorage()->debugGLStorage.emplace(walkingRootEntity->GetID(), debugGLRoot);

    AABBSystem::ConstructAABB(StorageManager::GetEntityById(walkingRootEntity->GetID()), glm::vec3(0.8,1.7,0.8), glm::vec3(0,0.8,0));
    AABBSystem::ConstructAABB(StorageManager::GetEntityById(testCube->GetID()), glm::vec3(1,1,1));

    Entity* cameraEnt = StorageManager::CreateCameraEntity();
    StorageManager::GetStorage()->entityStorage[cameraEnt->GetID()].SetParentID(walkingRootEntity->GetID());
    StorageManager::GetStorage()->entityStorage[walkingRootEntity->GetID()].SetChild(cameraEnt->GetID());
    RenderSystem::SetRenderCamera(StorageManager::GetEntityById(cameraEnt->GetID()));
    EngineDebug::AddCamera(cameraEnt);

    //Generates the world
    WorldGenerator::GenerateWorld(cameraEnt, new Shader("/Users/hayyan/Desktop/Repos/CPPEngine/VoxelEngine/src/Shaders/chunk.vert",
        "/Users/hayyan/Desktop/Repos/CPPEngine/VoxelEngine/src/Shaders/chunk.frag"));

    //Adds entities to physics system
    PhysicsSystem::Add(StorageManager::GetEntityById(walkingRootEntity->GetID()));
    PhysicsSystem::Add(StorageManager::GetEntityById(testCube->GetID()));

    //Loads entities into the renderer
    RenderSystem::Load(StorageManager::GetEntityById(testCube->GetID()));
    RenderSystem::Load(StorageManager::GetEntityById(walkingRootEntity->GetID() + 1));

    EngineDebug::Load(walkingRootEntity);
    EngineDebug::Load(testCube);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        AABBSystem::ConstructAABB(walkingRootEntity,
            StorageManager::GetStorage()->AABBStorage[walkingRootEntity->GetID()].size,
            StorageManager::GetStorage()->AABBStorage[walkingRootEntity->GetID()].offSet);

        AABBSystem::ConstructAABB(testCube,
            StorageManager::GetStorage()->AABBStorage[testCube->GetID()].size,
            StorageManager::GetStorage()->AABBStorage[testCube->GetID()].offSet);


        processInput(window, walkingRootEntity, deltaTime);
        PhysicsSystem::Run(deltaTime);

        TransformSystem::Update(StorageManager::GetEntityById(testCube->GetID()));
        TransformSystem::Update(StorageManager::GetEntityById(walkingRootEntity->GetID()));
        TransformSystem::Update(StorageManager::GetEntityById(cameraEnt->GetID()));

        CameraSystem::ConstructCamera(StorageManager::GetEntityById(cameraEnt->GetID()), glm::vec3(0, 2.0f, 0.0));
        StorageManager::GetStorage()->transformStorage[cameraEnt->GetID()].position = StorageManager::GetStorage()->transformStorage[walkingRootEntity->GetID()].position;

        Animator::RunAnimation(StorageManager::GetEntityById(walkingRootEntity->GetID()), "mixamo.com", deltaTime);

        WorldGenerator::RenderWorld(StorageManager::GetEntityById(walkingRootEntity->GetID()));
        RenderSystem::Draw(StorageManager::GetEntityById(testCube->GetID()));
        RenderSystem::Draw(StorageManager::GetEntityById(walkingRootEntity->GetID() + 1));

        EngineDebug::Draw(walkingRootEntity);
        EngineDebug::Draw(testCube);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window, Entity* ent, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        PlayerMovementSystem::Move(ent, Direction::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        PlayerMovementSystem::Move(ent, Direction::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        PlayerMovementSystem::Move(ent, Direction::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        PlayerMovementSystem::Move(ent, Direction::LEFT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}