#pragma once
#include "Storage.h"

class AABBSystem{
    public:
        AABBSystem();
        static void Init(Storage* storage);
        static void ConstructAABB(Entity* ent, glm::vec3 extents);
        static bool AABBVsAABBCollision(AABBComponent* entOne, AABBComponent* entTwo);
    private:
        static Storage* storage;
};