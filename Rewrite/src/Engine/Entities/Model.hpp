#pragma once
#include <Mesh.hpp>
#include <unordered_map>

struct ModelNode{
    std::string meshName;
    Mesh mesh;
};

class Model{
    public:
        Model();
        void AddMesh(std::string name, Mesh mesh);
        Mesh* GetMesh(std::string name);
        std::unordered_map<std::string, ModelNode>* GetNodes();

    private:
        std::unordered_map<std::string, ModelNode> nodes;

};