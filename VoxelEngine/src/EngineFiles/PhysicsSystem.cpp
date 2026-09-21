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
                //std::cout << "Colliding with {" << entities[j]->GetID() << "}" << std::endl;
                isColliding = true;
                //std::cout << "There has been a collision" << std::endl;
                break;
            }
        }

        if(!isColliding){
            //Need to do a broadphase of the blocks nearest to the entity
            //then send those entities to AABBSystem for AABBvsChunk
            //if it returns true do isColliding = true
            //Need to use the worldGenerator to get specific the specific chunk the entity is in
            Entity* currentChunk = WorldGenerator::GetChunk(transform.position);
            ChunkComponent& chunkComponent = StorageManager::GetStorage()->chunkStorage[currentChunk->GetID()];
            TransformComponent* chunkTransform = &StorageManager::GetStorage()->transformStorage[currentChunk->GetID()];
            
            //Now need to get the blocks closest to the AABB and then construct AABBs for the blocks nearby
            //Then do collision checks with the current AABB and the block AABB
            //Need to use modular to get the index positions
            int blockX = ((static_cast<int>(std::floor(transform.position.x)) % 16) + 16) % 16;
            int blockY = static_cast<int>(transform.position.y + entityAABB->offSet.y) ;
            int blockZ = ((static_cast<int>(std::floor(transform.position.z)) % 16) + 16) % 16;
            
            if(blockX < 16 && blockZ < 16 && blockY < 355){
                //Need to get a block it will be a cube like radius so the current AABB can be hit by 
                Block belowBlock = chunkComponent.blocks[blockX][blockY - 1][blockZ];

                if(belowBlock.blockID != 0){

                    //Need to get the blocks world pos for the collider so the currentAABB min and max can line up with the block
                    //Then I can construct the blockAABB using the blocks world pos
                    //Construct an AABB for the block that has a scale of 1
                    glm::vec3 blockWorldPos = glm::vec3(blockX, blockY - 1, blockZ) + chunkTransform->position;
                    //std::cout << "blockWoldPos Position:" << blockWorldPos.x << ", " <<blockWorldPos.y << ", " << blockWorldPos.z << std::endl;
                    AABBComponent blockAABB = AABBSystem::ConstructBlockAABB(blockWorldPos);

                    if(AABBSystem::AABBVsAABBCollision(entityAABB, &blockAABB)){
                        isColliding = true;
                        //std::cout << "Entity AABB is colliding with terrain" << std::endl;
                        AABBSystem::ConstructAABB(currentEntity, entityAABB->size, entityAABB->offSet);
                    }
                }
                else{
                    //std::cout << "Block underneath is air skipping check" << std::endl;
                }                

            }           
        }
        
        if(!isColliding){
            transform.position += gravity * deltaTime;
        }
    }    
}





