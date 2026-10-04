#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <assimp/Importer.hpp>
#include <FastNoise.h>
#include "Entity.hpp"
#include <Camera.hpp>
#include <WorldGenerator.hpp>
#include <BiomeStorage.hpp>
#include <TextureAtlasStorage.hpp>
#include <BlockStorage.hpp>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, Camera& cam){
    const float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.SetPosition(cam.GetPosition() + cameraSpeed * cam.GetFront());
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.SetPosition(cam.GetPosition() - cameraSpeed * cam.GetFront());
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.SetPosition(cam.GetPosition() - glm::normalize(glm::cross(cam.GetFront(), cam.GetUp())) * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.SetPosition(cam.GetPosition() + glm::normalize(glm::cross(cam.GetFront(), cam.GetUp())) * cameraSpeed);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cam.SetPosition(cam.GetPosition() + cameraSpeed * cam.GetUp());
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    GLFWwindow* window = glfwCreateWindow( 1280, 720, "A Pirates Tale", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD
    if (!gladLoadGLLoader(
        reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    Camera camera = Camera(glm::vec3(0,90, 0));

    ChunkRenderer::Load(&camera);
    BiomeStorage::Load();
    TextureAtlasStorage::Load();
    BlockStorage::Load();

    Entity player = Entity("Player");
    player.Load();
    player.Update();

    WorldGenerator::GenerateWorld(camera.GetPosition());

    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.2f, 0.4f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window, camera);
        WorldGenerator::RenderWorld(camera.GetPosition());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
