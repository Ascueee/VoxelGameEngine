#include "WorldGenerator.h"

std::vector<Entity*> WorldGenerator::chunkEntities;
WorldGenerator::WorldGenerator(){}

//Need to create a chunk Entity and 
void WorldGenerator::GenerateWorld(Entity* cameraEnt, Shader* shader){
    ChunkRenderer::AddShader(shader);
    ChunkRenderer::AddCamera(cameraEnt);

    //Builds the chunk and fills it with chunk data
    for(int x = 0; x < 33; x++){
        for(int z = 0; z < 33; z++){
            Entity* currentChunkEntity = StorageManager::CreateChunkEntity();
            TransformComponent& transform = StorageManager::GetStorage()->transformStorage[currentChunkEntity->GetID()];
            ChunkComponent& chunk = StorageManager::GetStorage()->chunkStorage[currentChunkEntity->GetID()];
            MaterialComponent& material = StorageManager::GetStorage()->materialStorage[currentChunkEntity->GetID()];

            StorageManager::GetStorage()->materialStorage[currentChunkEntity->GetID()].color = glm::vec3(1,1,1);
            StorageManager::GetStorage()->materialStorage[currentChunkEntity->GetID()].diffuse = EngineTextureLoader::GetTexture("Grass");
            transform.position = glm::vec3(x * chunk.width, transform.position.y, z * chunk.width);
            
            ChunkSystem::BuildChunk(currentChunkEntity);
            ChunkRenderer::Load(currentChunkEntity);
            chunkEntities.push_back(currentChunkEntity);
        }
    }

    //Second pass to give each chunk its neighbours for chunk Meshing/structure building
    for(int x = 0; x < 33; x++){
        for(int z = 0; z < 33; z++){
            Entity* currentChunkEntity = chunkEntities[33 * x + z];
            ChunkComponent& chunk = StorageManager::GetStorage()->chunkStorage[currentChunkEntity->GetID()];

            //Need to get the neighbouring chunk entities
            //Gets the front and back chunk entities
            if(z + 1 < 33)
                chunk.chunkNeighbours[0] = chunkEntities[ 33 * x + (z + 1)]->GetID();

            if(z - 1 >= 0)
                chunk.chunkNeighbours[1] = chunkEntities[ 33 * x + (z - 1)]->GetID();

            if(x + 1 < 33)
                //Gets the right and left chunk entities
                chunk.chunkNeighbours[2] = chunkEntities[ 33 * (x + 1) + z]->GetID();
            if(x - 1 >= 0)
                chunk.chunkNeighbours[3] = chunkEntities[ 33 * (x - 1) + z]->GetID();
        }
    }
}

//Will loop over all the chunks and render them in the scene
void WorldGenerator::RenderWorld(Entity* playerEnt){
    //Need to first translate player position into world position of the grid so x and y
    //then need to get the chunk entities surrounding the translated x and y and only render those
    //The radius depends on the render distance
    TransformComponent& trans = StorageManager::GetStorage()->transformStorage[playerEnt->GetID()];
    int voxelWorldPosX = trans.position.x / 16;
    int voxelWorldPosZ = trans.position.z / 16;
    int renderDistence = 16;

    //Renders only the chunks in the reder distance
    //The render distance is a sqaure radius that houses the chunks that need to be rendered
    for(int x = -renderDistence; x <= renderDistence; x++){
        for(int z = -renderDistence; z <= renderDistence; z++){
            
            int chunkIndex = 33 * (voxelWorldPosX + x) + (voxelWorldPosZ + z);

            if(chunkIndex < 0 || chunkIndex > chunkEntities.size())
                continue;

            TransformSystem::Update(chunkEntities[chunkIndex]);
            ChunkRenderer::Draw(chunkEntities[chunkIndex]);
        }
    }

}