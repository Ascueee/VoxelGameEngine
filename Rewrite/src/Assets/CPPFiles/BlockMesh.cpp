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

std::vector<unsigned int>& BlockMesh::GetFaceIndices(BlockFace face){
    if(face == BlockFace::TOP)
        return faceIndices.at("Top");
    else if(face == BlockFace::BOTTOM)
        return faceIndices.at("Bottom");
    else if(face == BlockFace::RIGHT)
        return faceIndices.at("Right");
    else if(face == BlockFace::LEFT)
        return faceIndices.at("Left");
    else if(face == BlockFace::FRONT)
        return faceIndices.at("Front");
    else if(face == BlockFace::BACK)
        return faceIndices.at("Back");
    else{
        return  faceIndices.at("Top");
    }
}