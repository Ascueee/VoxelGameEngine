#include "Block.hpp"

Block::Block(std::string name, unsigned int id, unsigned int textureId, BlockType type){
    this->name = name;
    this->id = id;
    this->textureId = textureId;
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

unsigned int& Block::GetTextureId(){
    return textureId;
}

BlockType& Block::GetType(){
    return type;
}


