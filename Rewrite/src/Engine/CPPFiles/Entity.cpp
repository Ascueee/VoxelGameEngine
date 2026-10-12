#include "Entity.hpp"

Entity::Entity(std::string name){
    this->name = name;
}

void Entity::Load(){
    std::cout << "[Entity] Loading Entity { " << name << "} \n"; 
}

void Entity::Update(){
    std::cout << "[Entity] Updating Entity { " << name << "} \n"; 
}
