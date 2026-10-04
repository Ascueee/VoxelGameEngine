#include <WorldGenerator.hpp>

std::unordered_map<glm::vec3, Chunk, Vec3Hash> WorldGenerator::chunks;
glm::vec3 WorldGenerator::lastPosition;
FastNoise WorldGenerator::tempreture;
FastNoise WorldGenerator::wetness;

int WorldGenerator::renderDistance = 20;
bool WorldGenerator::firstRun = true;
WorldGenerator::WorldGenerator(){}

//need to check the player position
//Also need a worldgenerator boolean called firstLoad which will load chunks around a starting position and also set the bounds to world gen
//Then need to check if the position is close to the bounds if so load chunks in render distance and delete the old chunks
//World Generator needs to go through the renderdistance and add to 
void WorldGenerator::GenerateWorld(glm::vec3 pos){
    std::filesystem::path dir = "/Users/hayyan/Desktop/Repos/CPPEngine/Rewrite/src/Engine/EngineData/WorldData/ChunkData";
    std::ifstream file;
    glm::vec3 center(std::floor(pos.x / 16.0f) * 16.0f, 0.0f, std::floor(pos.z / 16.0f) * 16.0f);
    lastPosition = center;
    glm::vec3 chunkCullBoundsMin = glm::vec3(center.x - renderDistance * 16.0f, center.y, center.z - renderDistance * 16.0f);
    glm::vec3 chunkCullBoundsMax = glm::vec3(center.x + renderDistance * 16.0f, center.y, center.z + renderDistance * 16.0f);

    tempreture.SetNoiseType(FastNoise::Perlin);
    tempreture.SetSeed(12345 + 300);
    tempreture.SetFrequency(0.1f);

    wetness.SetNoiseType(FastNoise::Perlin);
    wetness.SetSeed(12345 + 800);
    wetness.SetFrequency(0.1f);

    //Now that the chunks are generated and in the dictionary need to do a second pass where I go though each chunk
    //and see if they are in the render distence if not delete out of the dictionary to save ram
    //the min and max bounds are given off the player positon and uses the first pass of the -render distance and the max uses +renderdisance
    for (auto it = chunks.begin(); it != chunks.end(); )
    {
        glm::vec3 chunkPosition = it->second.GetPosition();

        if(chunkPosition.x < chunkCullBoundsMin.x || chunkPosition.z < chunkCullBoundsMin.z || 
            chunkPosition.x > chunkCullBoundsMax.x || chunkPosition.z > chunkCullBoundsMax.z){
            //std::cout << "[World Generator] Chunk Needs to be culled and deleted from dictionary\n";
            ChunkRenderer::Delete(it->second);
            it = chunks.erase(it);
        }
        else{
            ++it;
        }
    }
    std::cout << "[World Generator] Number of chunks: {" << chunks.size() << "} \n";

    //Before adding the chunk need to see if its in the render distance
    for(int x = -renderDistance; x < renderDistance; x++){
        for(int y = -renderDistance; y < renderDistance; y++){
            glm::vec3 offSet = glm::vec3(center.x + x * 16.0f, 0.0f, center.z + y * 16.0f);
            Chunk* chunk = nullptr;
            std::ifstream file;
            //need to create temp and wetness noise here
            //then send the biome to chunk which will change the frequency and have a multiplier to the noise
            int worldX = center.x + x;
            int worldZ = center.z + y;

            float tempretureNoise = (tempreture.GetNoise(worldX, worldZ) + 1.0f) * 50.0f;

            float wetnessNoise = (wetness.GetNoise(worldX, worldZ) + 1.0f) * 50.0f;

            //Checks if the chunks is already in the dictionary if so continue to the next chunk
            if(chunks.find(offSet) != chunks.end() && firstRun == false){
                continue;
            }

            std::filesystem::path fileName = "chunk_" + std::to_string((int)offSet.x) + "_" + 
                std::to_string((int)offSet.y) + "_" + std::to_string((int)offSet.z) + ".txt";
            
            if(std::filesystem::exists(dir / fileName)){
                file.open(dir/fileName);

                if(chunks.find(offSet) == chunks.end()){
                    chunks.emplace(offSet, Chunk(offSet,
                         BiomeStorage::GetBiomeByTempAndWetness(glm::vec2(tempretureNoise, wetnessNoise))));
                }

                chunk = &chunks.at(offSet);

                if(!file.is_open()){
                    std::cerr << "[World Generator] Could not open Chunk data" << (dir / fileName) << "\n";
                }
                else{
                    chunk->LoadChunk(file);
                }
            }
            else{
                chunks.emplace(offSet, Chunk(offSet, BiomeStorage::GetBiomeByName("Plains Islands")));
                chunk = &chunks.at(offSet);
                //Now need to check if the chunk has already been saved in the chunkData folder if so dont build the chunk again
                chunk->BuildChunk(offSet);
            }

            chunk->BuildMesh(offSet);
            ChunkRenderer::LoadMesh(chunks.at(offSet), pos);
        }
    }

    firstRun = false;
}

void WorldGenerator::RenderWorld(glm::vec3 pos){
    glm::vec3 center(std::floor(pos.x / 16.0f) * 16.0f, 0.0f, std::floor(pos.z / 16.0f) * 16.0f);
    
    //need to check for the min x and max x and if the offSet is at that bounds load the new chunks
    //Also do the same for y get the min and max and check if offSet is at the bounds if so generate
    int boundsDistence = 10;
    int minX = lastPosition.x - (renderDistance - boundsDistence) * 16;
    int maxX = lastPosition.x + (renderDistance - boundsDistence) * 16;
    int minZ = lastPosition.z - (renderDistance - boundsDistence) * 16;
    int maxZ = lastPosition.z + (renderDistance - boundsDistence) * 16;

    if(center.x > maxX || center.x < minX || center.z > maxZ || center.z < minZ){
        std::cout << "[WorldGenerator]  reached the bounds Update the world \n";
        GenerateWorld(pos);
    }

    for(int x = -renderDistance; x < renderDistance; x++){
        for(int y = -renderDistance; y < renderDistance; y++){
            glm::vec3 offSet = glm::vec3(lastPosition.x + x * 16.0f, 0.0f, lastPosition.z + y * 16.0f);


            //Updates the chunks events
            //chunks.at(offSet).UpdateChunk(pos);
            //if the chunk has been updated from an event or block place update the mesh
            if(chunks.at(offSet).GetIsGenerated() == false){
                chunks.at(offSet).BuildMesh(pos);
                chunks.at(offSet).SaveChunk();
                ChunkRenderer::LoadMesh(chunks.at(offSet), pos);
            }
            ChunkRenderer::Draw(chunks.at(offSet), pos);
            //std::cout << "[World Generator] Drawing Chunks \n"; 
        }
    }
}