#pragma once
#include <vector>
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
    private:
        static std::vector<Entity*> chunkEntities;

};