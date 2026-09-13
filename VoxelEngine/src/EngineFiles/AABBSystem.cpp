#include "AABBSystem.h"

Storage* AABBSystem::storage;

void AABBSystem::Init(Storage* newStorage){
    storage = newStorage;
}

//Creates the AABB for the entity
void AABBSystem::ConstructAABB(Entity* ent, glm::vec3 baseHalfExtents){
    TransformComponent& transform = storage->transformStorage[ent->GetID()];
    glm::vec3 halfExtents = baseHalfExtents * transform.scale;
    AABBComponent& aabb = storage->AABBStorage[ent->GetID()];
    aabb.center = transform.position;
    aabb.halfExtents = halfExtents;
    aabb.size = halfExtents;
    aabb.min = aabb.center - halfExtents;
    aabb.max = aabb.center + halfExtents;
}




bool AABBSystem::AABBVsAABBCollision(AABBComponent* entOne, AABBComponent* entTwo){
    
    return entOne->min.x <= entTwo->max.x &&
    entOne->max.x >= entTwo->min.x &&
    entOne->min.y <= entTwo->max.y &&
    entOne->max.y >= entTwo->min.y &&
    entOne->min.z <= entTwo->max.z &&
    entOne->max.z >= entTwo->min.z;
}



