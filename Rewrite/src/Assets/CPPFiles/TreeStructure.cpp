#include <TreeStructure.hpp>

TreeStructure::TreeStructure(){}

void TreeStructure::SetName(std::string name){
    this->name = name;
}

void TreeStructure::SetWidth(int width){
    this->width = width;
}

void TreeStructure::SetHeight(int height){
    this->height = height;
}

void TreeStructure::SetWoodId(unsigned int blockID){
    this->woodBlockId = blockID;
}

void TreeStructure::SetBlocks(std::unordered_map<std::string, ushort> blocks){
    this->blocks = blocks;
}

std::string TreeStructure::GetName(){
    return name;
}

std::unordered_map<std::string, ushort> TreeStructure::GetBlocks(){
    return blocks;
}

int TreeStructure::GetWidth(){
    return width;
}
int TreeStructure::GetHeight(){
    return height;
}

int TreeStructure::GetWoodBlockId(){
    return woodBlockId;
}