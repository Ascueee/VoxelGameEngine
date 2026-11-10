#pragma once
#include <iostream>
#include <glm/glm.hpp>

class TextureAtlas{
    public:
        TextureAtlas(std::string name, glm::ivec2 textureResolution, int textureSize);
        glm::vec4 GetUV(int textureID);
        unsigned int& GetHandle();
    private:
        std::string name;
        glm::ivec2 textureResolution;
        int textureSize;
        unsigned int handle;
};