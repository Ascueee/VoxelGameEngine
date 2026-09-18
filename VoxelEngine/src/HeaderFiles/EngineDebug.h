#pragma once
#include <glad/glad.h>
#include "StorageManager.h"
#include "Shader.h"


class EngineDebug{
    public:
        EngineDebug();
        static void Load(Entity* ent);
        static void Draw(Entity* ent);
        static void AddShader(Shader* newShader);
        static void AddCamera(Entity* ent);
    private:
        static Shader* shader;
        static int renderCamera;
};