#pragma once
#include "Entity.h"
#include "StorageManager.h"
#include "EngineBiomeLoader.h"
#include <vector>
#include <fastnoise/FastNoise.h>

enum BlockDirection{
    BLOCKFRONT,
    BLOCKBACK,
    BLOCKTOP,
    BLOCKBOTTOM,
    BLOCKLEFT,
    BLOCKRIGHT
};

class ChunkSystem{
    public: 
        ChunkSystem();
        static void BuildChunk(Entity* ent);
    private:
        static void BuildGrid(ChunkComponent& chunk, glm::vec2 chunkWorldPos);
        static void DecorateGrid(CameraComponent& chunk);
        static void BuildMesh(ChunkComponent& chunk, MeshComponent& mesh);
        static bool IsBlockSolid(Block block);
        static void GenerateCubeMesh(MeshComponent& mesh, BlockDirection dir, glm::vec3 blockPos);
};
