#pragma once
#include <Mesh.hpp>
#include <glad/glad.h>
#include <BlockModelStorage.hpp>
#include <Shader.h>
#include <AABB.hpp>

class DebugRenderer{
    public:
        DebugRenderer();
        static void Load();
        static void LoadAABB(Mesh& mesh);
        static void DrawAABB(Mesh& mesh, AABB* aabb, glm::vec3 color);
    private:
        static Shader shader;
};