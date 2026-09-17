#pragma once
#include "StorageManager.h"
#include "Shader.h"

class ChunkRenderer{
    public:
        ChunkRenderer();
        static void AddShader(Shader* newShader);
        static void Load(Entity* ent);
        static void Draw(Entity* ent);
        static void AddCamera(Entity* ent);
    private:
        static Shader* shader;
        static int renderCamera;
};