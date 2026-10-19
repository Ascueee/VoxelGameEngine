#include "Chunk.hpp"
#include "WorldGenerator.hpp"

Chunk::Chunk(){}

Chunk::Chunk(glm::vec3 position) : collider(position + glm::vec3(8, 128, 8), glm::vec3(16, 256, 16)){
    this->position = position;
    blocks.resize(width * height * width, (ushort)BlockStorage::GetBlock(0)->GetId());
    islandness.SetNoiseType(FastNoise::Perlin);
    islandness.SetSeed(12345);
    islandness.SetFrequency(0.001);  

    mountainess.SetNoiseType(FastNoise::Perlin);
    mountainess.SetSeed(12345 + 300);
    mountainess.SetFrequency(0.1f);

    tempreture.SetNoiseType(FastNoise::Perlin);
    tempreture.SetSeed(12345 + 500);
    tempreture.SetFrequency(0.01f); 
    
    wetness.SetNoiseType(FastNoise::Perlin);
    wetness.SetSeed(12345 + 800);
    wetness.SetFrequency(0.01f);  

    foliage.SetNoiseType(FastNoise::Perlin);
    foliage.SetSeed(12345 + 900);
    foliage.SetFrequency(0.1f);
}

void Chunk::BuildChunk(glm::vec3 pos){
    Biome* previousBiome;
    int previousHeight;

    //terrain pass
    for(int i = 0; i < blocks.size(); i++){
        int x = i % width;
        int y = i / (width * width);
        int z = (i / width) % width;

        int worldX = position.x + x;
        int worldZ = position.z + z;

        float temperatureNoise = ((tempreture.GetNoise(worldX, worldZ) + 1.0f) * 0.5f) * 100.0f;
        float wetnessNoise = ((wetness.GetNoise(worldX, worldZ) + 1.0f) * 0.5f) * 100.0f;

        biome = BiomeStorage::GetBiomeByTempAndWetness(glm::vec2(temperatureNoise, wetnessNoise));

        //if the y is above the max height the terrain can generate dont generate noise
        int maxHeight = 50 + biome->GetIslandness() + 15;
        if(y > maxHeight)
            continue;
        else if(y < 50){
            //will fill the bottom half of the terrain with stone
            blocks[i] = (ushort)BlockStorage::GetBlock(biome->GetBlockPallet()[1])->GetId();
            continue;
        }

        //Terrain gen stage
        float islandnessNoise = islandness.GetNoise(worldX, worldZ);
        float mountainNoise = mountainess.GetNoise(worldX, worldZ);

        //Need to fetch the biomes of the surrounding blocks
        //then need to blend between the biomes that differ
        Biome* leftBlockBiome =
            BiomeStorage::GetBiomeByTempAndWetness(glm::vec2(tempreture.GetNoise(worldX - 1, worldZ),
             wetness.GetNoise(worldX - 1, worldZ)));
        Biome* rightBlockBiome =
            BiomeStorage::GetBiomeByTempAndWetness(glm::vec2(tempreture.GetNoise(worldX + 1, worldZ),
             wetness.GetNoise(worldX + 1, worldZ)));
        Biome* frontBlockBiome =
            BiomeStorage::GetBiomeByTempAndWetness(glm::vec2(tempreture.GetNoise(worldX, worldZ + 1), 
            wetness.GetNoise(worldX, worldZ + 1)));
        Biome* backBlockBiome =
            BiomeStorage::GetBiomeByTempAndWetness(glm::vec2(tempreture.GetNoise(worldX, worldZ - 1),
             wetness.GetNoise(worldX, worldZ - 1)));
        
        int terrainHeight = 50 + static_cast<int>(islandnessNoise * biome->GetIslandness()) + static_cast<int>(mountainNoise * 15);

        if(leftBlockBiome->GetName() != biome->GetName()){
            int leftBiomeHeight = 
                50 + static_cast<int>(islandness.GetNoise(worldX - 1, worldZ) * leftBlockBiome->GetIslandness()) 
                + static_cast<int>(mountainess.GetNoise(worldX - 1, worldZ) * 15);
                terrainHeight = glm::mix(terrainHeight, leftBiomeHeight, 0.9f);
        }

        if(rightBlockBiome->GetName() != biome->GetName()){
            int rightBiomeHeight = 
                50 + static_cast<int>(islandness.GetNoise(worldX + 1, worldZ) * rightBlockBiome->GetIslandness())
                 + static_cast<int>(mountainess.GetNoise(worldX + 1, worldZ) * 15);
                terrainHeight = glm::mix(terrainHeight, rightBiomeHeight, 0.5f);
        }

        if(frontBlockBiome->GetName() != biome->GetName()){
            int frontBiomeHeight = 
                50 + static_cast<int>(islandness.GetNoise(worldX, worldZ + 1) * frontBlockBiome->GetIslandness())
                 + static_cast<int>(mountainess.GetNoise(worldX, worldZ + 1) * 15);
                terrainHeight = glm::mix(terrainHeight, frontBiomeHeight, 0.5f);
        }

        if(backBlockBiome->GetName() != biome->GetName()){
            int frontBiomeHeight = 
                50 + static_cast<int>(islandness.GetNoise(worldX, worldZ - 1) * backBlockBiome->GetIslandness())
                 + static_cast<int>(mountainess.GetNoise(worldX, worldZ - 1) * 15);
                terrainHeight = glm::mix(terrainHeight, frontBiomeHeight, 0.5f);
        }

        //Sets the blocks in the chunk
        if(y <= terrainHeight - 1){
            blocks[i] = (ushort)BlockStorage::GetBlock(biome->GetBlockPallet()[1])->GetId();
        }
        else if(y <= terrainHeight){
            blocks[i] = (ushort)BlockStorage::GetBlock(biome->GetBlockPallet()[0])->GetId();
        }
    }

    //Water pass for the terrain
    for(int i = 0; i < blocks.size(); i++){
        int y = i / (width * width);
        if(y <= 80 && BlockStorage::GetBlock(blocks[i])->GetType() == BlockType::AIR){
            blocks[i] = (ushort)BlockStorage::GetBlock(4)->GetId();
            //SubscribeBlockEvent(i, BlockEvents::LiquidBlock);
        }
    }

    //decoration stage
    //Biome decoration will have three stages:
    //Foliage pass: where grass and other foliage blocks will be placed
    //Biome store the foliage blocks in a dictionary and the key is their spawn
    //will make noise between 0-100
    //the key will be the value the noise needs to be to spawn the block
    for(int i = 0; i < blocks.size(); i++){
        int x = i % width;
        int y = i / (width * width);
        int z = (i / width) % width;

        //now need to use the biome to get the foliage that is closest to the value
        //also need to get the foliage spawn bounds so the y of the surface of the terrain and then the heighest y block from the surface
        //then only use those blocks for the foliage check
        int maxHeight = 50 + biome->GetIslandness() + 15;
        int blockBelow = i - width * width;
        //checks if the current block is on the surface and also if the current block is not air foliage can only spawn on air
        if(y > maxHeight || y < 50)
            continue;
        if(BlockStorage::GetBlock(blocks[i])->GetType() != BlockType::AIR)
            continue;
        if(BlockStorage::GetBlock(blocks[blockBelow])->GetType() != BlockType::SOLID)
            continue;

        int worldX = position.x + x;
        int worldZ = position.z + z;

        float temperatureNoise = ((tempreture.GetNoise(worldX, worldZ) + 1.0f) * 0.5f) * 100.0f;
        float wetnessNoise = ((wetness.GetNoise(worldX, worldZ) + 1.0f) * 0.5f) * 100.0f;
        biome = BiomeStorage::GetBiomeByTempAndWetness(glm::vec2(temperatureNoise, wetnessNoise));

        float foliageNoise = ((tempreture.GetNoise(worldX, worldZ) + 1.0f) * 0.5f) * 100.0f;

        //place the foliage block here using the biome to get the specific one needed to decorate the biome
        unsigned int decorationBlock = biome->GetDecorationBlock(foliageNoise);
        blocks[i] = BlockStorage::GetBlock(decorationBlock)->GetId();
    }

    //Tree passs: where the trees will be generated
    //will use a tree builder


    //Structure Stage
    //this is where general structures will be generated that are found in the biome
    //will use a structure builder
    
    SaveChunk();
}

void Chunk::BuildChunkMesh(glm::vec3 pos){
    terrainMesh.GetVertices().clear();
    terrainMesh.GetIndices().clear();

    foliageMesh.GetVertices().clear();
    foliageMesh.GetIndices().clear();
    //Vertex Layout: position 3, UVs 2(the uvs represent an index to the vec4 when creating the vertices)
    for(int i = 0; i < blocks.size(); i++){
        int currentBlock = blocks[i];
        int blockX = i % width;
        int blockY = i / (width * width);
        int blockZ = (i / width) % width;

        Mesh* meshPtr = &terrainMesh;
        if(BlockStorage::GetBlock(currentBlock)->GetType() == BlockType::FOLIAGE){
            meshPtr = &foliageMesh;
        }

        Mesh& mesh = *meshPtr;

        //checks if the current block is solid
        //if its solid generate the block mesh
        if(BlockStorage::GetBlock(blocks[i])->GetType() == BlockType::SOLID
         || BlockStorage::GetBlock(blocks[i])->GetType() == BlockType::LIQUID
         || BlockStorage::GetBlock(blocks[i])->GetType() == BlockType::FOLIAGE){
            //need to check if the block above the current block is air if its air then render
            int blockAbove = i + width * width;
            int blockBelow = i - width * width;

            int blockRight = i + 1;
            int blockLeft = i - 1;

            int blockFront = i + width;
            int blockBack  = i - width; 

            //gets the block model to use in the meshing process
            BlockMesh* blockMesh;
            if(BlockStorage::GetBlock(blocks[i])->GetType() == BlockType::FOLIAGE)
                blockMesh = BlockModelStorage::GetBlockModel("Grass.obj");
            else
                blockMesh = BlockModelStorage::GetBlockModel("Cube.obj");

            //Check if its not a cullable block build all the faces
            if(BlockStorage::GetBlock(currentBlock)->IsCullable() == false){
                //build the mesh with all its faces
                for (auto& face : blockMesh->GetFaceMap()){
                    if(face.first == "Top")
                        BuildMesh(mesh, blockMesh, currentBlock, glm::vec3(blockX,blockY,blockZ), BlockFace::TOP);
                    else if(face.first == "Bottom")
                        BuildMesh(mesh, blockMesh, currentBlock, glm::vec3(blockX,blockY,blockZ), BlockFace::BOTTOM);
                    else if(face.first == "Right")
                        BuildMesh(mesh, blockMesh, currentBlock, glm::vec3(blockX,blockY,blockZ), BlockFace::RIGHT);
                    else if(face.first == "Left")
                        BuildMesh(mesh, blockMesh, currentBlock, glm::vec3(blockX,blockY,blockZ), BlockFace::LEFT);
                    else if(face.first == "Right")
                        BuildMesh(mesh, blockMesh, currentBlock, glm::vec3(blockX,blockY,blockZ), BlockFace::FRONT);
                    else if(face.first == "Back")
                        BuildMesh(mesh, blockMesh, currentBlock, glm::vec3(blockX,blockY,blockZ), BlockFace::BACK);
                }
                //skip the rest because the mesh is already built
                continue;
            }

            //need to cull the side of the chunks by checking neighbouring chunks
            //if the neighbouring block on the neighburing chunk is air render the mesh
            //also only do this check on the edge blocks of the chunk
            //Front and Back
            if(blockY == height - 1 || BlockStorage::GetBlock(blocks[blockAbove])->GetType() == BlockType::AIR
                || BlockStorage::GetBlock(blocks[blockAbove])->GetType() == BlockType::FOLIAGE){
                BuildMesh(mesh, blockMesh, currentBlock, glm::vec3(blockX,blockY,blockZ), BlockFace::TOP);
            }

            if(blockY == 0 || BlockStorage::GetBlock(blocks[blockBelow])->GetType() == BlockType::AIR
                || BlockStorage::GetBlock(blocks[blockBelow])->GetType() == BlockType::FOLIAGE){
                BuildMesh(mesh, blockMesh, currentBlock, glm::vec3(blockX,blockY,blockZ), BlockFace::BOTTOM);
            }

            if(blockX != width - 1){
                if(BlockStorage::GetBlock(blocks[blockRight])->GetType() == BlockType::AIR
                 || BlockStorage::GetBlock(blocks[blockRight])->GetType() == BlockType::FOLIAGE){
                    BuildMesh(mesh, blockMesh, currentBlock, glm::vec3(blockX,blockY,blockZ), BlockFace::RIGHT);
                }
            }
            else{
                //this means that this is an edge block need to check neighbouring chunk to see
                //if the neighbouring block is air render the face 
                glm::vec3 neighbourChunkPosition = glm::vec3(position.x + 16, position.y, position.z);
                Chunk* neighbourChunk = WorldGenerator::GetChunk(neighbourChunkPosition);

                int neighbourBlockIndex;
                Block* neighbourBlock;

                if(neighbourChunk != nullptr){
                    //now that I have the neighbouring chunk need to get the block thats next to the current block
                    //z = width - 1 
                    neighbourBlockIndex = 0 + (blockZ * width) + (width * width * blockY);
                    neighbourBlock = BlockStorage::GetBlock(neighbourChunk->blocks[neighbourBlockIndex]);
                }
                else{
                    neighbourBlock = BlockStorage::GetBlock(blocks[i]);
                }

                if(neighbourBlock->GetType() == BlockType::AIR || neighbourBlock->GetType() == BlockType::FOLIAGE){
                    BuildMesh(mesh, blockMesh, currentBlock, glm::vec3(blockX,blockY,blockZ), BlockFace::RIGHT);
                }
            }

            if(blockX != 0){
                if(BlockStorage::GetBlock(blocks[blockLeft])->GetType() == BlockType::AIR
                    || BlockStorage::GetBlock(blocks[blockLeft])->GetType() == BlockType::FOLIAGE){
                    BuildMesh(mesh, blockMesh, currentBlock, glm::vec3(blockX,blockY,blockZ), BlockFace::LEFT);
                }
            }
            else{
                //this means that this is an edge block need to check neighbouring chunk to see
                //if the neighbouring block is air render the face 
                glm::vec3 neighbourChunkPosition = glm::vec3(position.x - 16, position.y, position.z);
                Chunk* neighbourChunk = WorldGenerator::GetChunk(neighbourChunkPosition);

                int neighbourBlockIndex;
                Block* neighbourBlock;

                if(neighbourChunk != nullptr){
                    //now that I have the neighbouring chunk need to get the block thats next to the current block
                    //z = width - 1 
                    neighbourBlockIndex = (width - 1) + (blockZ * width) + (width * width * blockY);
                    neighbourBlock = BlockStorage::GetBlock(neighbourChunk->blocks[neighbourBlockIndex]);
                }
                else{
                    neighbourBlock = BlockStorage::GetBlock(blocks[i]);
                }

                if(neighbourBlock->GetType() == BlockType::AIR || neighbourBlock->GetType() == BlockType::FOLIAGE){
                    BuildMesh(mesh, blockMesh, currentBlock, glm::vec3(blockX,blockY,blockZ), BlockFace::LEFT);
                }
            }

            //need to switch outer loop need to check if its on the edge and then check if the block is air switch
            if(blockZ != width - 1){
                if(BlockStorage::GetBlock(blocks[blockFront])->GetType() == BlockType::AIR
                    || BlockStorage::GetBlock(blocks[blockFront])->GetType() == BlockType::FOLIAGE){
                    BuildMesh(mesh, blockMesh, currentBlock, glm::vec3(blockX,blockY,blockZ), BlockFace::FRONT);
                }
            }
            else{
                //this means that this is an edge block need to check neighbouring chunk to see
                //if the neighbouring block is air render the face 
                glm::vec3 neighbourChunkPosition = glm::vec3(position.x, position.y, (position.z + 16));
                Chunk* neighbourChunk = WorldGenerator::GetChunk(neighbourChunkPosition);

                int neighbourBlockIndex;
                Block* neighbourBlock;

                if(neighbourChunk != nullptr){
                    //now that I have the neighbouring chunk need to get the block thats next to the current block
                    //z = width - 1 
                    neighbourBlockIndex = blockX + (0 * width) + (width * width * blockY);
                    neighbourBlock = BlockStorage::GetBlock(neighbourChunk->blocks[neighbourBlockIndex]);
                }
                else{
                    neighbourBlock = BlockStorage::GetBlock(blocks[i]);
                }

                //checks if the neighboyr block is air if so render face
                if(neighbourBlock->GetType() == BlockType::AIR || neighbourBlock->GetType() == BlockType::FOLIAGE){
                    BuildMesh(mesh, blockMesh, currentBlock, glm::vec3(blockX,blockY,blockZ), BlockFace::FRONT);
                }
            }
 
            if(blockZ != 0){
                if(BlockStorage::GetBlock(blocks[blockBack])->GetType() == BlockType::AIR
                    || BlockStorage::GetBlock(blocks[blockBack])->GetType() == BlockType::FOLIAGE){
                    BuildMesh(mesh, blockMesh, currentBlock, glm::vec3(blockX,blockY,blockZ), BlockFace::BACK);
                }
            }
            else{
                //this means that this is an edge block need to check neighbouring chunk to see
                //if the neighbouring block is air render the face 
                glm::vec3 neighbourChunkPosition = glm::vec3(position.x, position.y,(position.z - 16));
                Chunk* neighbourChunk = WorldGenerator::GetChunk(neighbourChunkPosition);

                int neighbourBlockIndex;
                Block* neighbourBlock;

                if(neighbourChunk != nullptr){
                    //now that I have the neighbouring chunk need to get the block thats next to the current block
                    //z = width - 1 
                    neighbourBlockIndex = blockX + ((width - 1) * width) + (width * width * blockY);
                    neighbourBlock = BlockStorage::GetBlock(neighbourChunk->blocks[neighbourBlockIndex]);
                }
                else{
                    neighbourBlock = BlockStorage::GetBlock(blocks[i]);
                }

                //checks if the neighboyr block is air if so render face
                if(neighbourBlock->GetType() == BlockType::AIR || neighbourBlock->GetType() == BlockType::FOLIAGE){
                    BuildMesh(mesh, blockMesh, currentBlock, glm::vec3(blockX,blockY,blockZ), BlockFace::BACK);
                }
            }
        }
    }

    isGenerated = true;
}


//Two types of block models: cullable and non cullable
//non cullable: use all the faces no matter what so the mesh will loop through all faces and add it to the mesh
//cullable:: will use the mesh system of using each face and checking the neighbouring blocks
//block models using this must follow the mesh guide lines in blender with having six meshes for each side
//and naming them accordingly
//This will be in one function and the chunkBuildMesh Will call it
void Chunk::BuildMesh(Mesh& mesh, BlockMesh* blockMesh, int block, glm::vec3 blockPos, BlockFace blockFace){
    std::vector<float> vertices = blockMesh->GetVertices();
    std::vector<unsigned int> faceIndices = blockMesh->GetFaceIndices(blockFace);
    //loop through each face and build the model
    glm::vec4 textureUVs = 
    TextureAtlasStorage::GetTexture("BaseAtlas")->GetUV(BlockStorage::GetBlock(block)->GetTextureId(blockFace));
    unsigned int baseVertex = mesh.GetVertices().size() / 8;
    //Need to loop over the indices in this case top face indices
    //Then need to add the vertices to the mesh with the position offset
    for(int faceIndex = 0; faceIndex < 6; faceIndex++){
        mesh.GetVertices().push_back(vertices[faceIndices[faceIndex] * 8] + blockPos.x);
        mesh.GetVertices().push_back(vertices[faceIndices[faceIndex] * 8 + 1] + blockPos.y);
        mesh.GetVertices().push_back(vertices[faceIndices[faceIndex] * 8 + 2] + blockPos.z);

        float uvX = vertices[faceIndices[faceIndex] * 8 + 3];
        float uvY = vertices[faceIndices[faceIndex] * 8 + 4];

        mesh.GetVertices().push_back(glm::mix(textureUVs.x, textureUVs.z, uvX)); 
        mesh.GetVertices().push_back(glm::mix(textureUVs.y, textureUVs.w, uvY));

        mesh.GetVertices().push_back(vertices[faceIndices[faceIndex] * 8 + 5]);
        mesh.GetVertices().push_back(vertices[faceIndices[faceIndex] * 8 + 6]);
        mesh.GetVertices().push_back(vertices[faceIndices[faceIndex] * 8 + 7]);
    }

    for(int i = 0; i < 6; i++){
        mesh.GetIndices().push_back(baseVertex + i);
    }  
}

void Chunk::UpdateChunk(glm::vec3 pos){
    //auto events = blockEvents; 
    // for (auto& event : events)
    // {
    //    event.second(*this, event.first);
    // }
}

//Only save blocks that are not 
void Chunk::SaveChunk(){
    std::string fileName = "chunk_" + std::to_string((int)position.x) + "_" +
    std::to_string((int)position.y) + "_" +
    std::to_string((int)position.z) +
    ".txt";
    std::filesystem::path savePath =
        "/Users/hayyan/Desktop/Repos/CPPEngine/Rewrite/src/Engine/EngineData/WorldData/ChunkData";

    std::ofstream file(savePath / fileName);

    for(int i = 0; i < blocks.size(); i++){
        if(BlockStorage::GetBlock(blocks[i])->GetType() == BlockType::AIR)
            continue;
        
        file << i << " " << BlockStorage::GetBlock(blocks[i])->GetId() << "\n";
    }
}

//Loop through all the blocks in the chunk save data
//And add them to the block index to reconstruct the chunk
void Chunk::LoadChunk(std::ifstream& file){
    int blockIndex;
    int blockId;
    while(file >> blockIndex >> blockId){
        if(blockIndex >= 0 && blockIndex < blocks.size() && blockId != 0){
            blocks[blockIndex] = blockId;
        }
    }
}

void Chunk::PlaceBlock(glm::vec3 pos, unsigned int id){
    int blockIndex = pos.x + width * (pos.z + width * pos.y);
    blocks[blockIndex] = (ushort)BlockStorage::GetBlock(id)->GetId();
    SubscribeBlockEvent(blockIndex, BlockEvents::LiquidBlock);
    isGenerated = false; 
}

void Chunk::SubscribeBlockEvent(int blockIndex, std::function<void(Chunk&, int)> event){
    blockEvents.emplace(blockIndex, event);
}

void Chunk::SetIsGenerated(bool isGenerated){
    this->isGenerated = isGenerated;
}

Mesh& Chunk::GetTerrainMesh(){
    return terrainMesh;
}

Mesh& Chunk::GetFoliageMesh(){
    return foliageMesh;
}

Mesh& Chunk::GetDebugMesh(){
    return debugMesh;
}

AABB* Chunk::GetCollider(){
    return &collider;
}

std::vector<ushort>& Chunk::GetBlocks(){
    return blocks;
}

glm::vec3 Chunk::GetPosition(){
    return position;
}

bool Chunk::GetIsGenerated(){
    return isGenerated;
}