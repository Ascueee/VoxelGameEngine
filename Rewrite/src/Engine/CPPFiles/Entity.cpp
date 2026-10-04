#include "Entity.hpp"

Entity::Entity(std::string name){
    this->name = name;
}

void Entity::Load(){
    std::cout << "Loading Entity { " << name << "} \n"; 
}

void Entity::Update(){
    std::cout << "Updating Entity { " << name << "} \n"; 
}
