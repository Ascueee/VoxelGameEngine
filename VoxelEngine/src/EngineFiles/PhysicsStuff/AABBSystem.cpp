#include "AABBSystem.h"

Storage* AABBSystem::storage;

void AABBSystem::Init(Storage* newStorage){
    storage = newStorage;
}

//Creates the AABB for the entity
void AABBSystem::ConstructAABB(Entity* ent, glm::vec3 size, glm::vec3 offSet){
    TransformComponent& transform = StorageManager::GetStorage()->transformStorage[ent->GetID()];
    AABBComponent& aabb = StorageManager::GetStorage()->AABBStorage[ent->GetID()];

    aabb.offSet = offSet;
    aabb.center = transform.position + aabb.offSet;
    aabb.size = size;
    aabb.halfSize = size * 0.5f;
    aabb.min = aabb.center - aabb.halfSize;
    aabb.max = aabb.center + aabb.halfSize;
}

//Takes in the blocks world pos and constructs an AABB using the world pos for the center
//Returns the AABB component because its not going to be an entity because its going to get thrown out after the collision check
AABBComponent AABBSystem::ConstructBlockAABB(glm::vec3 blockWorldPos){
    AABBComponent blockAABB;

    blockAABB.offSet = glm::vec3(0);
    blockAABB.size = glm::vec3(1.0f, 0.51f, 1.0f);
    blockAABB.halfSize = blockAABB.size * 0.5f;

    blockAABB.min = blockWorldPos;
    blockAABB.max = blockWorldPos + blockAABB.size;
    blockAABB.center = blockWorldPos + blockAABB.halfSize;

    return blockAABB;
}
bool AABBSystem::AABBVsAABBCollision(AABBComponent* entOne, AABBComponent* entTwo){
    
    return entOne->min.x <= entTwo->max.x &&
    entOne->max.x >= entTwo->min.x &&
    entOne->min.y <= entTwo->max.y &&
    entOne->max.y >= entTwo->min.y &&
    entOne->min.z <= entTwo->max.z &&
    entOne->max.z >= entTwo->min.z;
}







