#include "Chunk.hpp"

Chunk::Chunk(){}

Chunk::Chunk(glm::vec3 position){
    this->position = position;
    blocks.resize(width * height * width, (ushort)BlockStorage::GetBlock(0)->GetId());
    islandness.SetNoiseType(FastNoise::Perlin);
    islandness.SetSeed(12345);
    islandness.SetFrequency(0.01);  

    mountainess.SetNoiseType(FastNoise::Perlin);
    mountainess.SetSeed(12345 + 300);
    mountainess.SetFrequency(0.1f);

    tempreture.SetNoiseType(FastNoise::Perlin);
    tempreture.SetSeed(12345 + 500);
    tempreture.SetFrequency(0.001f); 
    
    wetness.SetNoiseType(FastNoise::Perlin);
    wetness.SetSeed(12345 + 800);
    wetness.SetFrequency(0.001f);  
}

void Chunk::BuildChunk(glm::vec3 pos){
    Biome* previousBiome;
    int previousHeight;

    for(int i = 0; i < blocks.size(); i++){
        int x = i % width;
        int y = i / (width * width);
        int z = (i / width) % width;

        int worldX = position.x + x;
        int worldZ = position.z + z;


        float islandnessNoise = islandness.GetNoise(worldX, worldZ);
        float mountainNoise = mountainess.GetNoise(worldX, worldZ);
        float tempretureNoise = (tempreture.GetNoise(worldX, worldZ));
        float wetnessNoise = (wetness.GetNoise(worldX, worldZ));

        biome = BiomeStorage::GetBiomeByTempAndWetness(glm::vec2(tempretureNoise, wetnessNoise));

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


    SaveChunk();
}

void Chunk::BuildMesh(glm::vec3 pos){
    mesh.GetVertices().clear();
    mesh.GetIndices().clear();

    //Vertex Layout: position 3, UVs 2(the uvs represent an index to the vec4 when creating the vertices)
    for(int i = 0; i < blocks.size(); i++){
        int x = i % width;
        int y = i / (width * width);
        int z = (i / width) % width;

        //checks if the current block is solid
        //if its solid generate the block mesh
        if(BlockStorage::GetBlock(blocks[i])->GetType() == BlockType::SOLID
         || BlockStorage::GetBlock(blocks[i])->GetType() == BlockType::LIQUID){
            //need to check if the block above the current block is air if its air then render
            int blockAbove = i + width * width;
            int blockBelow = i - width * width;

            int blockRight = i + 1;
            int blockLeft = i - 1;

            int blockFront = i + width;
            int blockBack  = i - width; 

            
            //gets the block model to use in the meshing process
            BlockMesh* blockMesh = BlockModelStorage::GetBlockModel("Cube.obj");
            std::vector<float> vertices = blockMesh->GetVertices();
            std::vector<unsigned int> topFaceIndices = blockMesh->GetFaceIndices("Top");
            std::vector<unsigned int> bottomFaceIndices = blockMesh->GetFaceIndices("Bottom");
            std::vector<unsigned int> frontFaceIndices = blockMesh->GetFaceIndices("Front");
            std::vector<unsigned int> backFaceIndices = blockMesh->GetFaceIndices("Back");
            std::vector<unsigned int> rightFaceIndices = blockMesh->GetFaceIndices("Right");
            std::vector<unsigned int> leftFaceIndices = blockMesh->GetFaceIndices("Left");

            //Front and Back
            if(y == height - 1 || BlockStorage::GetBlock(blocks[blockAbove])->GetType() == BlockType::AIR){
                //Use a textureAtlas to get the UVS depending on the textureID
                glm::vec4 textureUVs = 
                TextureAtlasStorage::GetTexture("BaseAtlas")->GetUV(BlockStorage::GetBlock(blocks[i])->GetTextureId(BlockFace::TOP));

                unsigned int baseVertex = mesh.GetVertices().size() / 5;
                //Need to loop over the indices in this case top face indices
                //Then need to add the vertices to the mesh with the position offset
                for(int faceIndex = 0; faceIndex < 6; faceIndex++){
                    mesh.GetVertices().push_back(vertices[topFaceIndices[faceIndex] * 5] + x);
                    mesh.GetVertices().push_back(vertices[topFaceIndices[faceIndex] * 5 + 1] + y);
                    mesh.GetVertices().push_back(vertices[topFaceIndices[faceIndex] * 5 + 2] + z);

                    float uvX = vertices[topFaceIndices[faceIndex] * 5 + 3];
                    float uvY = vertices[topFaceIndices[faceIndex] * 5 + 4];

                    mesh.GetVertices().push_back(glm::mix(textureUVs.x, textureUVs.z, uvX)); 
                    mesh.GetVertices().push_back(glm::mix(textureUVs.y, textureUVs.w, uvY));

                }

                for(int i = 0; i < 6; i++){
                    mesh.GetIndices().push_back(baseVertex + i);
                }
            }

            if(y == 0 || BlockStorage::GetBlock(blocks[blockBelow])->GetType() == BlockType::AIR){
                glm::vec4 textureUVs = 
                TextureAtlasStorage::GetTexture("BaseAtlas")->GetUV(BlockStorage::GetBlock(blocks[i])->GetTextureId(BlockFace::BOTTOM));
                unsigned int baseVertex = mesh.GetVertices().size() / 5;
                //Need to loop over the indices in this case top face indices
                //Then need to add the vertices to the mesh with the position offset
                for(int faceIndex = 0; faceIndex < 6; faceIndex++){
                    mesh.GetVertices().push_back(vertices[bottomFaceIndices[faceIndex] * 5] + x);
                    mesh.GetVertices().push_back(vertices[bottomFaceIndices[faceIndex] * 5 + 1] + y);
                    mesh.GetVertices().push_back(vertices[bottomFaceIndices[faceIndex] * 5 + 2] + z);

                    float uvX = vertices[bottomFaceIndices[faceIndex] * 5 + 3];
                    float uvY = vertices[bottomFaceIndices[faceIndex] * 5 + 4];

                    mesh.GetVertices().push_back(glm::mix(textureUVs.x, textureUVs.z, uvX)); 
                    mesh.GetVertices().push_back(glm::mix(textureUVs.y, textureUVs.w, uvY));
                }

                for(int i = 0; i < 6; i++){
                    mesh.GetIndices().push_back(baseVertex + i);
                }
            }

            if(x == width - 1 || BlockStorage::GetBlock(blocks[blockRight])->GetType() == BlockType::AIR){
                glm::vec4 textureUVs = 
                TextureAtlasStorage::GetTexture("BaseAtlas")->GetUV(BlockStorage::GetBlock(blocks[i])->GetTextureId(BlockFace::RIGHT));
                unsigned int baseVertex = mesh.GetVertices().size() / 5;
                //Need to loop over the indices in this case top face indices
                //Then need to add the vertices to the mesh with the position offset
                for(int faceIndex = 0; faceIndex < 6; faceIndex++){
                    mesh.GetVertices().push_back(vertices[rightFaceIndices[faceIndex] * 5] + x);
                    mesh.GetVertices().push_back(vertices[rightFaceIndices[faceIndex] * 5 + 1] + y);
                    mesh.GetVertices().push_back(vertices[rightFaceIndices[faceIndex] * 5 + 2] + z);

                    float uvX = vertices[rightFaceIndices[faceIndex] * 5 + 3];
                    float uvY = vertices[rightFaceIndices[faceIndex] * 5 + 4];

                    mesh.GetVertices().push_back(glm::mix(textureUVs.x, textureUVs.z, uvX)); 
                    mesh.GetVertices().push_back(glm::mix(textureUVs.y, textureUVs.w, uvY));
                }

                for(int i = 0; i < 6; i++){
                    mesh.GetIndices().push_back(baseVertex + i);
                }
            }

            if(x == 0 || BlockStorage::GetBlock(blocks[blockLeft])->GetType() == BlockType::AIR){
                glm::vec4 textureUVs = 
                TextureAtlasStorage::GetTexture("BaseAtlas")->GetUV(BlockStorage::GetBlock(blocks[i])->GetTextureId(BlockFace::LEFT));
                unsigned int baseVertex = mesh.GetVertices().size() / 5;
                //Need to loop over the indices in this case top face indices
                //Then need to add the vertices to the mesh with the position offset
                for(int faceIndex = 0; faceIndex < 6; faceIndex++){
                    mesh.GetVertices().push_back(vertices[leftFaceIndices[faceIndex] * 5] + x);
                    mesh.GetVertices().push_back(vertices[leftFaceIndices[faceIndex] * 5 + 1] + y);
                    mesh.GetVertices().push_back(vertices[leftFaceIndices[faceIndex] * 5 + 2] + z);

                    float uvX = vertices[leftFaceIndices[faceIndex] * 5 + 3];
                    float uvY = vertices[leftFaceIndices[faceIndex] * 5 + 4];

                    mesh.GetVertices().push_back(glm::mix(textureUVs.x, textureUVs.z, uvX)); 
                    mesh.GetVertices().push_back(glm::mix(textureUVs.y, textureUVs.w, uvY));
                }

                for(int i = 0; i < 6; i++){
                    mesh.GetIndices().push_back(baseVertex + i);
                }
            }

            if(z == width - 1 || BlockStorage::GetBlock(blocks[blockFront])->GetType() == BlockType::AIR){
                glm::vec4 textureUVs = 
                TextureAtlasStorage::GetTexture("BaseAtlas")->GetUV(BlockStorage::GetBlock(blocks[i])->GetTextureId(BlockFace::FRONT));
                unsigned int baseVertex = mesh.GetVertices().size() / 5;
                //Need to loop over the indices in this case top face indices
                //Then need to add the vertices to the mesh with the position offset
                for(int faceIndex = 0; faceIndex < 6; faceIndex++){
                    mesh.GetVertices().push_back(vertices[frontFaceIndices[faceIndex] * 5] + x);
                    mesh.GetVertices().push_back(vertices[frontFaceIndices[faceIndex] * 5 + 1] + y);
                    mesh.GetVertices().push_back(vertices[frontFaceIndices[faceIndex] * 5 + 2] + z);

                    float uvX = vertices[frontFaceIndices[faceIndex] * 5 + 3];
                    float uvY = vertices[frontFaceIndices[faceIndex] * 5 + 4];

                    mesh.GetVertices().push_back(glm::mix(textureUVs.x, textureUVs.z, uvX)); 
                    mesh.GetVertices().push_back(glm::mix(textureUVs.y, textureUVs.w, uvY));
                }

                for(int i = 0; i < 6; i++){
                    mesh.GetIndices().push_back(baseVertex + i);
                }
            }
 
            if(z == 0 || BlockStorage::GetBlock(blocks[blockBack])->GetType() == BlockType::AIR){
                glm::vec4 textureUVs = 
                TextureAtlasStorage::GetTexture("BaseAtlas")->GetUV(BlockStorage::GetBlock(blocks[i])->GetTextureId(BlockFace::BACK));
                unsigned int baseVertex = mesh.GetVertices().size() / 5;
                //Need to loop over the indices in this case top face indices
                //Then need to add the vertices to the mesh with the position offset
                for(int faceIndex = 0; faceIndex < 6; faceIndex++){
                    mesh.GetVertices().push_back(vertices[backFaceIndices[faceIndex] * 5] + x);
                    mesh.GetVertices().push_back(vertices[backFaceIndices[faceIndex] * 5 + 1] + y);
                    mesh.GetVertices().push_back(vertices[backFaceIndices[faceIndex] * 5 + 2] + z);

                    float uvX = vertices[backFaceIndices[faceIndex] * 5 + 3];
                    float uvY = vertices[backFaceIndices[faceIndex] * 5 + 4];

                    mesh.GetVertices().push_back(glm::mix(textureUVs.x, textureUVs.z, uvX)); 
                    mesh.GetVertices().push_back(glm::mix(textureUVs.y, textureUVs.w, uvY));
                }

                for(int i = 0; i < 6; i++){
                    mesh.GetIndices().push_back(baseVertex + i);
                }
            }
        }
    }

    isGenerated = true;
}

void Chunk::UpdateChunk(glm::vec3 pos){
    auto events = blockEvents; 
    for (auto& event : events)
    {
       event.second(*this, event.first);
    }
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

Mesh& Chunk::GetMesh(){
    return mesh;
}

std::vector<ushort>& Chunk::GetBlocks(){
    return blocks;
}

glm::vec3& Chunk::GetPosition(){
    return position;
}

bool Chunk::GetIsGenerated(){
    return isGenerated;
}