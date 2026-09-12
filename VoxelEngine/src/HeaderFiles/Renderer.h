#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Entity.h"
#include "Storage.h"

class Renderer{
    public:
        Renderer();
        static void Init(Storage* storage, Shader* shader);
        static void Load(Entity* ent);
        static void Draw(Entity* ent);
        static Storage* storage;
    private:
        static Shader* shader;
};