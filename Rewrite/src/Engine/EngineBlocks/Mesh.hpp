#pragma once
#include <iostream>
#include <Texture.hpp>

class Mesh{
    public:
        Mesh();
        std::vector<float>& GetVertices();
        std::vector<unsigned int>& GetIndices();
        void SetTexture(Texture* texture);
        Texture* GetTexture();
        unsigned int& GetVBO();
        unsigned int& GetVAO();
        unsigned int& GetEBO();
    private:
        unsigned int vbo;
        unsigned int vao;
        unsigned int ebo;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        Texture* texture;
};