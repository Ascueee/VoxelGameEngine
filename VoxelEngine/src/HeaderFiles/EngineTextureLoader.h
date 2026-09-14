#pragma once
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <glad/glad.h>
#include <stb_image.h>
#include "Texture.h"

class EngineTextureLoader{
    public:
        EngineTextureLoader();
        static void Load();
        static Texture* GetTexture(std::string textureName);
    private:
        static std::unordered_map<std::string, Texture> textures;

};