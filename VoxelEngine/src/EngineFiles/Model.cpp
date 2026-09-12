#include "Model.h"

Model::Model(){}

Model::Model(std::string modelName){
    this->modelName = modelName;
}

std::vector<ModelNode>& Model::GetNodes(){
    return nodes;
}

std::string& Model::GetModelName(){
    return modelName;
}

Rig& Model::GetRig(){
    return rig;
}

