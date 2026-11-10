#pragma once
#include <iostream>
#include <unordered_map>
#include <Mesh.hpp>
#include <Model.hpp>
#include <filesystem>
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class ModelStorage{
    public:
        ModelStorage();
        static void Load();
        static Model* GetModel(std::string name);
    private:
        static std::unordered_map<std::string, Model> models;
        static void ProcessNode(aiNode* node, const aiScene* scene, std::string modelName);
        static Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, Model* model, std::string meshName);
};