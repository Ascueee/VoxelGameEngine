#include <Texture.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(){}

void Texture::SetName(std::string name){
    this->name = name;
}

void Texture::SetType(TextureType type){
    this->type = type;
}

unsigned int& Texture::GetHandle(){
    return handle;
}

TextureType Texture::GetType(){
    return type;
}

std::string Texture::GetName(){
    return name;
}
