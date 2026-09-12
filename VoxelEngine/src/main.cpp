#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include "StorageManager.h"
#include "EngineModelLoader.h"
#include "System.h"
#include "TransformSystem.h"
#include "Animator.h"
#include "Renderer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, float deltaTime);

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
StorageManager manager;
TransformSystem* transformSystem;
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

    transformSystem = new TransformSystem(manager.GetStorage());
    Animator::storage = manager.GetStorage();
    Renderer::Init(manager.GetStorage(), new Shader("/Users/hayyan/Desktop/Repos/CPPEngine/VoxelEngine/src/Shaders/base.vert",
        "/Users/hayyan/Desktop/Repos/CPPEngine/VoxelEngine/src/Shaders/base.frag"));

    manager.CreateBlankEntity();
    manager.CreateCubeEntity();
    manager.GetStorage()->colorStorage[1].color = glm::vec3(0,0,1);
    manager.GetStorage()->transformStorage[1].scale = glm::vec3(10,1,10);

    manager.CreateCubeEntity();
    manager.GetStorage()->colorStorage[2].color = glm::vec3(1,0,1);
    manager.GetStorage()->transformStorage[2].position = glm::vec3(-2,1,0);
    manager.GetStorage()->transformStorage[2].scale = glm::vec3(1,1,1);

    manager.BuildModel(EngineModelLoader::GetModel("Walking"));

    manager.GetStorage()->transformStorage[4].scale = glm::vec3(0.01f);
    manager.GetStorage()->transformStorage[4].position = glm::vec3(0,1,0);
    
    Renderer::Load(manager.GetEntityById(1));
    Renderer::Load(manager.GetEntityById(2));
    Renderer::Load(manager.GetEntityById(4));

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
        transformSystem->Run();

        Animator::RunAnimation(manager.GetEntityById(3), "mixamo.com", deltaTime);
        Renderer::Draw(manager.GetEntityById(1));
        Renderer::Draw(manager.GetEntityById(2));
        Renderer::Draw(manager.GetEntityById(4));

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