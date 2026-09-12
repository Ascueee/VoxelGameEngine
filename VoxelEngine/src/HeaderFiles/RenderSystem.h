#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Entity.h"
#include "Storage.h"

class RenderSystem{
    public:
        RenderSystem();
        static void Init(Storage* storage, Shader* shader);
        static void Load(Entity* ent);
        static void Draw(Entity* ent);
        
    private:
        static Storage* storage;
        static Shader* shader;
};