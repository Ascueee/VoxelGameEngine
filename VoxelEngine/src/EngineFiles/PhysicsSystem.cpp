#include "PhysicsSystem.h"

Storage* PhysicsSystem::storage;
std::vector<Entity*> PhysicsSystem::entities;
glm::vec3 PhysicsSystem::gravity = glm::vec3(0.0f, -1.0f, 0.0f);

void PhysicsSystem::Add(Entity* ent){
    entities.push_back(ent);
}

void PhysicsSystem::Init(Storage* newStorage){
    storage = newStorage;
}

void PhysicsSystem::Run(float deltaTime){
    for(int i = 0; i < entities.size(); i++){
        //Now need to add gravity to the objects with a collider
        Entity* currentEntity = &storage->entityStorage[entities[i]->GetID()];
        TransformComponent& transform = storage->transformStorage[currentEntity->GetID()];
        AABBComponent* entityAABB = &storage->AABBStorage[currentEntity->GetID()];
        PhysicsComponent& physicsComponent = storage->physicsStorage[currentEntity->GetID()];

        if(physicsComponent.isStatic == true)
            continue;
        
        bool isColliding = false;
        for(int j = 0; j < entities.size(); j++){
            if(currentEntity->GetID() == entities[j]->GetID())
                continue;
            
            AABBComponent* entityTwoAABB = &storage->AABBStorage[entities[j]->GetID()];
            if(AABBSystem::AABBVsAABBCollision(entityAABB, entityTwoAABB)){
                isColliding = true;
                //std::cout << "There has been a collision" << std::endl;
                break;
            }
        }


        if(!isColliding){
            transform.position += gravity * deltaTime;
            AABBSystem::ConstructAABB(currentEntity, entityAABB->halfExtents);
        }
    }    

}

