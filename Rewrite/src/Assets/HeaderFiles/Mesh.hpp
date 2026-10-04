#pragma once

#include <iostream>
class Mesh{
    public:
        Mesh();
        std::vector<float>& GetVertices();
        std::vector<unsigned int>& GetIndices();
        unsigned int& GetVBO();
        unsigned int& GetVAO();
        unsigned int& GetEBO();

    private:
        unsigned int vbo;
        unsigned int vao;
        unsigned int ebo;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
};