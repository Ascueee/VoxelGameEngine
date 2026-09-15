#pragma once
#include "StorageManager.h"

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