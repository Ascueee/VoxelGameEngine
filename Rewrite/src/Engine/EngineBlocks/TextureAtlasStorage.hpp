#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <glad/glad.h>
#include <TextureAtlas.hpp>

class TextureAtlasStorage{
    public:
        TextureAtlasStorage();
        static void Load();
        static TextureAtlas* GetTexture(std::string name);
    private:
        static std::unordered_map<std::string, TextureAtlas> textures;
};