#include "Entity.h"

Entity::Entity(){}

void Entity::SetID(int id){
    this->id = id;
}

void Entity::SetParentID(int parentID){
    this->parentID = parentID;
}

void Entity::SetName(std::string name){
    this->name = name;
}

std::string Entity::GetName(){
    return name;
}

void Entity::SetChild(int childID){
    children.push_back(childID);
}

int Entity::GetID() const{
    return id;
}

int Entity::GetParentID(){
    return parentID;
}

std::vector<int> Entity::GetChildren(){
    return children;
}