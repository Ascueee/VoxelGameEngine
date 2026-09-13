#pragma once
#include "Storage.h"
#include "AABBSystem.h"
#include <vector>

class PhysicsSystem{
    public:
        PhysicsSystem();
        static void Init(Storage* storage);
        static void Add(Entity* ent);
        static void Run(float deltaTime);
    private:
        static Storage* storage;
        static std::vector<Entity*> entities;
        static glm::vec3 gravity;
};