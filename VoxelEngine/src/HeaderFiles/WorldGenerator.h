#pragma once
#include <vector>
#include <cmath>
#include "StorageManager.h"
#include "ChunkSystem.h"
#include "ChunkRenderer.h"
#include "TransformSystem.h"
#include "EngineTextureLoader.h"

class WorldGenerator{
    public:
        WorldGenerator();
        static void GenerateWorld(Entity* cameraEnt, Shader* shader);
        static void RenderWorld(Entity* playerEnt);
        static Entity* GetChunk(glm::vec3 worldPos);
    private:
        static std::vector<Entity*> chunkEntities;

};