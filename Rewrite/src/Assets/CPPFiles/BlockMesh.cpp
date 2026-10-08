#include <BlockMesh.hpp>
BlockMesh::BlockMesh(){}

void BlockMesh::AddFace(std::string name, std::vector<unsigned int> indices){
    faceIndices.emplace(name, indices);
}

std::vector<float>& BlockMesh::GetVertices(){
    return vertices;
}

std::vector<unsigned int>& BlockMesh::GetFaceIndices(std::string face){
    return faceIndices.at(face);
}