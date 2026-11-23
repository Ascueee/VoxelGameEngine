#include "Mesh.hpp"

Mesh::Mesh(){}

std::vector<float>& Mesh::GetVertices(){
    return vertices;
}

std::vector<unsigned int>& Mesh::GetIndices(){
    return indices;
}

void Mesh::SetTexture(Texture* texture){
    this->texture = texture;
}

Texture* Mesh::GetTexture(){
    return texture;
}

unsigned int& Mesh::GetVBO(){
    return vbo;
}

unsigned int& Mesh::GetVAO(){
    return vao;
}

unsigned int& Mesh::GetEBO(){
    return ebo;
}