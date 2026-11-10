#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <Entity.hpp>
#include <Camera.hpp>
#include <WorldGenerator.hpp>
#include <BiomeStorage.hpp>
#include <TextureAtlasStorage.hpp>
#include <BlockStorage.hpp>
#include <BlockModelStorage.hpp>
#include <ModelStorage.hpp>
#include <DebugRenderer.hpp>
#include <TreeBuilder.hpp>
#include <StructureStorage.hpp>
#include <ScriptStorage.hpp>
#include <sol/sol.hpp>
#include <Transform.hpp>
#include <CameraController.hpp>
#include <MeshController.hpp>

//ENGINE NOTES:
//THIS IS A VOXEL GAME ENGINE
//THERE ARE TWO MAIN PARTS TO THE ENGINE
//VOXEL WORLD GENERATOR: Chunks, Blocks, structures
//ENTITIES: Game objects thay interact with the world terrain(Player, NPC, Monster, Animal, etc)
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// void processInput(GLFWwindow *window, Camera& cam){

//     const float cameraSpeed = 0.5f; // adjust accordingly
//     if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//         cam.SetPosition(cam.GetPosition() + cameraSpeed * cam.GetFront());
//     if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//         cam.SetPosition(cam.GetPosition() - cameraSpeed * cam.GetFront());
//     if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//         cam.SetPosition(cam.GetPosition() - glm::normalize(glm::cross(cam.GetFront(), cam.GetUp())) * cameraSpeed);
//     if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//         cam.SetPosition(cam.GetPosition() + glm::normalize(glm::cross(cam.GetFront(), cam.GetUp())) * cameraSpeed);

//     if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
//         cam.SetPosition(cam.GetPosition() + cameraSpeed * cam.GetUp());

//     if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
//         cam.SetPosition(cam.GetPosition() - cameraSpeed * cam.GetUp());
// }

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

    Renderer::Load();
    //DebugRenderer::Load(&camera);
    
    TextureAtlasStorage::Load();
    BlockStorage::Load();
    BiomeStorage::Load();
    BlockModelStorage::Load();
    ModelStorage::Load();
    StructureStorage::Load();
    ScriptStorage::Load();
    TreeBuilder::Load();

    sol::state lua;
    Entity player = Entity();
    player.SetName("Player");
    player.AddPackage<Transform>();
    player.AddPackage<CameraController>();
    player.AddPackage<MeshController>();
    player.GetPackage<Transform>()->position = glm::vec3(1000,400,30);
    player.GetPackage<MeshController>()->mesh = ModelStorage::GetModel("Test")->GetMesh("Cone");
    player.Load();
    player.Update();

    //divides the camera position by blockSize to keep the position consistant with the world
    WorldGenerator::GenerateWorld(player.GetPackage<Transform>()->position / 4.0f);

    Renderer::SetCamera(player.GetPackage<CameraController>());
    Renderer::LoadMesh(*player.GetPackage<MeshController>()->mesh);
    Renderer::LoadGBuffer();

    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.2f, 0.4f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

        sol::protected_function_result result = ScriptStorage::GetScript("test")(&player);
        if(!result.valid()){
            sol::error err = result;
            std::cout << "[Script] " << err.what() << "\n";
        }

        //processInput(window, camera);

        Renderer::InitializeGeometryPass();
        Renderer::GeometryPassEntity(*player.GetPackage<MeshController>()->mesh, player.GetPackage<Transform>());
        WorldGenerator::DeferredRenderWorld(player.GetPackage<Transform>()->position / 4.0f, fbWidth, fbHeight);
        Renderer::DeferredPass(fbWidth, fbHeight);
        Renderer::InitializeForwardPass(fbWidth, fbHeight);
        WorldGenerator::ForwardRenderPass();
        Renderer::ForwardPassEntity(*player.GetPackage<MeshController>()->mesh, player.GetPackage<Transform>());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
