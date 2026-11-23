#pragma once
#include <Texture.hpp>

class Material{
    public:
        Material();
        void SetTexture(Texture* texture);
        Texture* GetTexture();
    private:
        Texture* texture;
};