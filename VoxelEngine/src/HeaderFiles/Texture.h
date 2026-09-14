#pragma once
#include <iostream>
#include <glad/glad.h>

class Texture{
    public: 
        Texture();
        Texture(std::string textureName);
        unsigned int GetHandle();
    private:
        std::string textureName;
        unsigned int handle;
};