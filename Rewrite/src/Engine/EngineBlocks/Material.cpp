#include <Material.hpp>

Material::Material(){}

void Material::SetTexture(Texture* texture){
    this->texture = texture;
}

Texture* Material::GetTexture(){
    return texture;
}
