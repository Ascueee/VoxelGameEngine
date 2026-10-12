#pragma once
#include <glad/glad.h>
#include "Chunk.hpp"
#include "Shader.h"
#include <Camera.hpp>
#include <TextureAtlasStorage.hpp>

class Chunk;

class ChunkRenderer{
    public:
        ChunkRenderer();
        static void Load(Camera* camera);
        static void LoadMesh(Chunk& chunk);
        static void Draw(Chunk& chunk);
        static void Delete(Chunk& chunk);
    private:
        static Shader shader;
        static Camera* camera;
        static unsigned int gBuffer;
};