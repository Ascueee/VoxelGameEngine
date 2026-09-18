#pragma once
#include "StorageManager.h"
#include "AABBSystem.h"

enum Direction{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class PlayerMovementSystem{
    public:
        PlayerMovementSystem();
        static void Move(Entity* ent, Direction dir, float deltaTime);
    private:
};