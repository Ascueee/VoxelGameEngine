#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <Texture.hpp>

class TextureAtlas : public Texture{
    public:
        TextureAtlas(glm::ivec2 textureResolution, int textureSize);
        glm::vec4 GetUV(int textureID);
    private:
        glm::ivec2 textureResolution;
        int textureSize;
};