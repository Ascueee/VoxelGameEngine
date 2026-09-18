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
bool AABBSystem::AABBVsAABBCollision(AABBComponent* entOne, AABBComponent* entTwo){
    
    return entOne->min.x <= entTwo->max.x &&
    entOne->max.x >= entTwo->min.x &&
    entOne->min.y <= entTwo->max.y &&
    entOne->max.y >= entTwo->min.y &&
    entOne->min.z <= entTwo->max.z &&
    entOne->max.z >= entTwo->min.z;
}




