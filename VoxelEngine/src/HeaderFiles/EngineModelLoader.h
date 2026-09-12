#pragma once
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>
#include <unordered_map>
#include "Model.h"
#include "Animation.h"

class EngineModelLoader{
    public:
        EngineModelLoader();
        static void Load(); //this will load all the models in the model folder
        static Model* GetModel(std::string modelName);
    private:
        static void ProcessNode(aiNode *node, const aiScene *scene, int parentID, std::string model);
        static void ProcessMesh(aiMesh *mesh, const aiScene *scene, ModelNode& modelNode, std::string modelName);
        static void ProcessTransform(aiNode* node, ModelNode& modelNode);
        static void ExtractRig(aiMesh* mesh, const aiScene* scene, std::vector<float>& vertices, std::string modelName);
        static void ProcessAnimation(const aiScene* scene, Model* model);
        static std::unordered_map<std::string, Model> models;
        
};