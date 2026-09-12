#pragma once
#include "Storage.h"
class TransformSystem{
    public:
        TransformSystem();
        static void Init(Storage* storage);
        static void Update(Entity* ent);
    private:
        static Storage* storage;
};