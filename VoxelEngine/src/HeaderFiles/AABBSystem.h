#pragma once
#include "StorageManager.h"

class AABBSystem{
    public:
        AABBSystem();
        static void Init(Storage* storage);
        static void ConstructAABB(Entity* ent, glm::vec3 size = glm::vec3(1,1,1), glm::vec3 offSet = glm::vec3(0,0,0));
        static AABBComponent ConstructBlockAABB(glm::vec3 blockWorldPos);
        static bool AABBVsAABBCollision(AABBComponent* entOne, AABBComponent* entTwo);
        static bool AABBVsChunkCollision(AABBComponent* ent, ChunkComponent* currentChunk);
    private:
        static Storage* storage;
};