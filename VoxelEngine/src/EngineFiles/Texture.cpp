#include "Texture.h"
Texture::Texture(){}

Texture::Texture(std::string textureName){
    this->textureName = textureName;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);
}

unsigned int Texture::GetHandle(){
    return handle;
}