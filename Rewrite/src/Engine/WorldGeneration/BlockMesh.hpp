#pragma once
#include <Mesh.hpp>
#include <iostream>
#include <unordered_map>
#include <Block.hpp>

class BlockMesh{
    public:
        BlockMesh();
        void AddFace(std::string face, std::vector<unsigned int> indices);
        std::vector<float>& GetVertices();
        std::vector<unsigned int>& GetFaceIndices(std::string face);
        std::vector<unsigned int>& GetFaceIndices(BlockFace face);
        std::unordered_map<std::string, std::vector<unsigned int>>& GetFaceMap();
    private:
        std::vector<float> vertices;
        //the key will be the meshname in blender(NAMES NEED TO BE: TOP,BOTTOM,RIGHT,LEFT,FRONT,BACK) for chunkmeshing
        std::unordered_map<std::string, std::vector<unsigned int>> faceIndices;
};