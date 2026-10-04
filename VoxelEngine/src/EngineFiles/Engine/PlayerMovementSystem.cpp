#include "PlayerMovementSystem.h"

PlayerMovementSystem::PlayerMovementSystem(){}

void PlayerMovementSystem::Move(Entity* ent, Direction dir, float deltaTime){
    TransformComponent& trans = StorageManager::GetStorage()->transformStorage[ent->GetID()];
    PlayerMovement* move = &StorageManager::GetStorage()->playerMovementStorage[ent->GetID()];
    AABBComponent* aabbComponent = &StorageManager::GetStorage()->AABBStorage[ent->GetID()];
    PhysicsComponent& physics = StorageManager::GetStorage()->physicsStorage[ent->GetID()];
    if(dir == Direction::FORWARD){
        physics.velocity += trans.front * move->speed * deltaTime;
        //trans.position += trans.front * move->speed * deltaTime;
    }
    else if(dir == Direction::BACKWARD){
        physics.velocity -= trans.front * move->speed * deltaTime;
        //trans.position -= trans.front * move->speed * deltaTime;
    }
    else if(dir == Direction::RIGHT){
        physics.velocity += glm::normalize(glm::cross(trans.front, trans.up)) * move->speed * deltaTime;
        //trans.position += glm::normalize(glm::cross(trans.front, trans.up)) * move->speed * deltaTime;
    }
    else if(dir == Direction::LEFT){
        physics.velocity -= glm::normalize(glm::cross(trans.front, trans.up)) * move->speed * deltaTime;
       // trans.position -= glm::normalize(glm::cross(trans.front, trans.up)) * move->speed * deltaTime;
    }
}