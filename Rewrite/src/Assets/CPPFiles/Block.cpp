#include "Block.hpp"

Block::Block(std::string name, unsigned int id, std::vector<unsigned int> textureIds, BlockType type){
    this->name = name;
    this->id = id;
    this->textureIds = textureIds;
    this->type = type;
}

void Block::SetId(unsigned int id){
    this->id = id;
}

std::string& Block::GetName(){
    return name;
}
unsigned int& Block::GetId(){
    return id;
}

unsigned int& Block::GetTextureId(BlockFace face){
    return textureIds[face];
}

BlockType& Block::GetType(){
    return type;
}


