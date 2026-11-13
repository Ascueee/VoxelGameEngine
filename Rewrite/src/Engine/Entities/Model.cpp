#include <Model.hpp>

Model::Model(){}

void Model::AddMesh(std::string name, Mesh mesh){
    ModelNode node;
    node.meshName = name;
    node.mesh = mesh;
    nodes.emplace(name, node);
}

Mesh* Model::GetMesh(std::string name){
    if(nodes.find(name) != nodes.end()){
        return &nodes.at(name).mesh;
    }
    else{
        return &nodes["Cube"].mesh;
    }
}

std::unordered_map<std::string, ModelNode>* Model::GetNodes(){
    return &nodes;
}