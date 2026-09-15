#include "PlayerMovementSystem.h"

PlayerMovementSystem::PlayerMovementSystem(){}

void PlayerMovementSystem::Move(Entity* ent, Direction dir, float deltaTime){
    TransformComponent& trans = StorageManager::GetStorage()->transformStorage[ent->GetID()];
    PlayerMovement* move = &StorageManager::GetStorage()->playerMovementStorage[ent->GetID()];
    if(dir == Direction::FORWARD){
        trans.position += trans.front * move->speed * deltaTime;
    }
    else if(dir == Direction::BACKWARD){
        trans.position -= trans.front * move->speed * deltaTime;
    }
    else if(dir == Direction::RIGHT){
        trans.position += glm::normalize(glm::cross(trans.front, trans.up)) * move->speed * deltaTime;
    }
    else if(dir == Direction::LEFT){
        trans.position -= glm::normalize(glm::cross(trans.front, trans.up)) * move->speed * deltaTime;
    }
}