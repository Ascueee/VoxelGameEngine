#pragma once
#include <vector>
#include "Component.h"
#include "Rig.h"


struct ModelNode{
    std::string nodeName;
    int parentId = -1;
    int id = 0;
    std::vector<int> children;
    MeshComponent modelMesh;
    TransformComponent modelTransform;
};

//Stores ModelNodes which preserve the models hierarchy and mesh
//Used by the engine to load the model and meshComponenets to an entity heirarchy in StorageManager
class Model{
    public:
        Model();
        Model(std::string modelName);
        std::vector<ModelNode>& GetNodes();
        std::string& GetModelName();
        Rig& GetRig();
    private:
        std::string modelName;
        std::vector<ModelNode> nodes;
        Rig rig;
};