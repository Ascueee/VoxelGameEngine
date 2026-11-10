#include "Entity.hpp"

Entity::Entity(){}

void Entity::SetName(std::string name){
    this->name = name;
}


std::string Entity::GetName(){
    return name;
}

void Entity::Load(){
    std::cout << "[Entity] Loading Entity { " << name << "} \n"; 
}

void Entity::Update(){
    std::cout << "[Entity] Updating Entity { " << name << "} \n"; 
}

Entity::~Entity() {
    for (auto& pair : packages) {
        delete pair.second;
    }
}
