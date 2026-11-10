#pragma once
#include <unordered_map>
#include <BlockMesh.hpp>
#include <filesystem>
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class BlockModelStorage{
    public:
        BlockModelStorage();
        static void Load();
        static BlockMesh* GetBlockModel(std::string modelName);
    private:
        //saves the block model by a name ex: "Cube" = cubeModelMesh
        static std::unordered_map<std::string, BlockMesh> models;
        static void ProcessNode(aiNode *node, const aiScene *scene, std::string modelName);
        static void ProcessBlockMesh(BlockMesh& blockMesh, aiMesh *mesh, const aiScene *scene);
};