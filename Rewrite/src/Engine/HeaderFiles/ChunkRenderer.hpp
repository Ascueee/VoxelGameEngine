#pragma once
#include <glad/glad.h>
#include "Chunk.hpp"
#include "Shader.h"
#include <Camera.hpp>
#include <TextureAtlasStorage.hpp>

class ChunkRenderer{
    public:
        ChunkRenderer();
        static void Load(Camera* camera);
        static void LoadMesh(Chunk& chunk, glm::vec3 pos);
        static void Draw(Chunk& chunk, glm::vec3 pos);
        static void Delete(Chunk& chunk);
    private:
        static Shader shader;
        static Camera* camera;
};