#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <glad/glad.h>
#include <Texture.hpp>
#include <unordered_map>




class TextureStorage{
    public:
        TextureStorage();
        static void Load();
        static Texture* GetTexture(std::string name);
    private:
        static std::unordered_map<std::string, Texture> textures;
};
