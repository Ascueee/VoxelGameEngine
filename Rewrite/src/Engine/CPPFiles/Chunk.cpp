#include "Chunk.hpp"

Chunk::Chunk(){}

Chunk::Chunk(glm::vec3 position, Biome* biome){
    this->position = position;
    this->biome = biome;
    blocks.resize(width * height * width, (ushort)BlockStorage::GetBlock(0)->GetId());
    islandness.SetNoiseType(FastNoise::Perlin);
    islandness.SetSeed(12345);
    islandness.SetFrequency(0.01);  

    mountainess.SetNoiseType(FastNoise::Perlin);
    mountainess.SetSeed(12345 + 300);
    mountainess.SetFrequency(0.1f);  
}

void Chunk::BuildChunk(glm::vec3 pos){
    
    for(int i = 0; i < blocks.size(); i++){
        int x = i % width;
        int y = i / (width * width);
        int z = (i / width) % width;

        int worldX = position.x + x;
        int worldZ = position.z + z;

        float islandnessNoise = islandness.GetNoise(static_cast<float>(worldX), static_cast<float>(worldZ));
        float mountainNoise = islandness.GetNoise(static_cast<float>(worldX), static_cast<float>(worldZ));

        int terrainHeight = 50 + static_cast<int>(islandnessNoise * biome->GetIslandness()) + static_cast<int>(mountainNoise * 15);
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
            SubscribeBlockEvent(i, BlockEvents::LiquidBlock);
        }
    }

    SaveChunk();
}

void Chunk::BuildMesh(glm::vec3 pos){
    mesh.GetVertices().clear();
    mesh.GetIndices().clear();

    //std::cout << "[Chunk]Building ChunkMesh \n";
    //Vertex Layout: position 3, UVs 2(the uvs represent an index to the vec4 when creating the vertices)
    float vertices[] = {
        // TOP (+Y)
        -0.5f,  0.5f, -0.5f, 0, 1, // 0
        0.5f,  0.5f, -0.5f, 2, 1, // 1
        0.5f,  0.5f,  0.5f, 2, 3, // 2
        -0.5f,  0.5f,  0.5f, 0, 3, // 3

        // BOTTOM (-Y)
        -0.5f, -0.5f,  0.5f, 0, 1, // 4
        0.5f, -0.5f,  0.5f, 2, 1, // 5
        0.5f, -0.5f, -0.5f, 2, 3, // 6
        -0.5f, -0.5f, -0.5f, 0, 3, // 7

        // FRONT (+Z)
        -0.5f,  0.5f,  0.5f, 0, 1, // 8
        0.5f,  0.5f,  0.5f, 2, 1, // 9
        0.5f, -0.5f,  0.5f, 2, 3, // 10
        -0.5f, -0.5f,  0.5f, 0, 3, // 11

        // BACK (-Z)
        0.5f,  0.5f, -0.5f, 0, 1, // 12
        -0.5f,  0.5f, -0.5f, 2, 1, // 13
        -0.5f, -0.5f, -0.5f, 2, 3, // 14
        0.5f, -0.5f, -0.5f, 0, 3, // 15

        // RIGHT (+X)
        0.5f,  0.5f,  0.5f, 0, 1, // 16
        0.5f,  0.5f, -0.5f, 2, 1, // 17
        0.5f, -0.5f, -0.5f, 2, 3, // 18
        0.5f, -0.5f,  0.5f, 0, 3, // 19

        // LEFT (-X)
        -0.5f,  0.5f, -0.5f, 0, 1, // 20
        -0.5f,  0.5f,  0.5f, 2, 1, // 21
        -0.5f, -0.5f,  0.5f, 2, 3, // 22
        -0.5f, -0.5f, -0.5f, 0, 3  // 23
    };
    unsigned int topFaceIndices[]    = { 0, 3, 2, 2, 1, 0 };
    unsigned int bottomFaceIndices[] = { 4, 7, 6, 6, 5, 4 };
    unsigned int frontFaceIndices[]  = { 8, 11, 10, 10, 9, 8 };
    unsigned int backFaceIndices[]   = { 12, 15, 14, 14, 13, 12 };
    unsigned int rightFaceIndices[]  = { 16, 19, 18, 18, 17, 16 };
    unsigned int leftFaceIndices[]   = { 20, 23, 22, 22, 21, 20 };

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

            //Use a textureAtlas to get the UVS depending on the textureID
            glm::vec4 textureUVs = TextureAtlasStorage::GetTexture("BaseAtlas")->GetUV(BlockStorage::GetBlock(blocks[i])->GetTextureId());

            //Front and Back
            if(y == height - 1 || BlockStorage::GetBlock(blocks[blockAbove])->GetType() == BlockType::AIR){
                unsigned int baseVertex = mesh.GetVertices().size() / 5;
                //Need to loop over the indices in this case top face indices
                //Then need to add the vertices to the mesh with the position offset
                for(int faceIndex = 0; faceIndex < 6; faceIndex++){
                    mesh.GetVertices().push_back(vertices[topFaceIndices[faceIndex] * 5] + x);
                    mesh.GetVertices().push_back(vertices[topFaceIndices[faceIndex] * 5 + 1] + y);
                    mesh.GetVertices().push_back(vertices[topFaceIndices[faceIndex] * 5 + 2] + z);

                    //Add uvs to mesh
                    mesh.GetVertices().push_back(textureUVs[vertices[topFaceIndices[faceIndex] * 5 + 3]]);
                    mesh.GetVertices().push_back(textureUVs[vertices[topFaceIndices[faceIndex] * 5 + 4]]);

                }

                for(int i = 0; i < 6; i++){
                    mesh.GetIndices().push_back(baseVertex + i);
                }
            }

            if(y == 0 || BlockStorage::GetBlock(blocks[blockBelow])->GetType() == BlockType::AIR){
                unsigned int baseVertex = mesh.GetVertices().size() / 5;
                //Need to loop over the indices in this case top face indices
                //Then need to add the vertices to the mesh with the position offset
                for(int faceIndex = 0; faceIndex < 6; faceIndex++){
                    mesh.GetVertices().push_back(vertices[bottomFaceIndices[faceIndex] * 5] + x);
                    mesh.GetVertices().push_back(vertices[bottomFaceIndices[faceIndex] * 5 + 1] + y);
                    mesh.GetVertices().push_back(vertices[bottomFaceIndices[faceIndex] * 5 + 2] + z);

                    mesh.GetVertices().push_back(textureUVs[vertices[bottomFaceIndices[faceIndex] * 5 + 3]]);
                    mesh.GetVertices().push_back(textureUVs[vertices[bottomFaceIndices[faceIndex] * 5 + 4]]);
                }

                for(int i = 0; i < 6; i++){
                    mesh.GetIndices().push_back(baseVertex + i);
                }
            }

            if(x == width - 1 || BlockStorage::GetBlock(blocks[blockRight])->GetType() == BlockType::AIR){
                unsigned int baseVertex = mesh.GetVertices().size() / 5;
                //Need to loop over the indices in this case top face indices
                //Then need to add the vertices to the mesh with the position offset
                for(int faceIndex = 0; faceIndex < 6; faceIndex++){
                    mesh.GetVertices().push_back(vertices[rightFaceIndices[faceIndex] * 5] + x);
                    mesh.GetVertices().push_back(vertices[rightFaceIndices[faceIndex] * 5 + 1] + y);
                    mesh.GetVertices().push_back(vertices[rightFaceIndices[faceIndex] * 5 + 2] + z);

                    mesh.GetVertices().push_back(textureUVs[vertices[rightFaceIndices[faceIndex] * 5 + 3]]);
                    mesh.GetVertices().push_back(textureUVs[vertices[rightFaceIndices[faceIndex] * 5 + 4]]);
                }

                for(int i = 0; i < 6; i++){
                    mesh.GetIndices().push_back(baseVertex + i);
                }
            }

            if(x == 0 || BlockStorage::GetBlock(blocks[blockLeft])->GetType() == BlockType::AIR){
                unsigned int baseVertex = mesh.GetVertices().size() / 5;
                //Need to loop over the indices in this case top face indices
                //Then need to add the vertices to the mesh with the position offset
                for(int faceIndex = 0; faceIndex < 6; faceIndex++){
                    mesh.GetVertices().push_back(vertices[leftFaceIndices[faceIndex] * 5] + x);
                    mesh.GetVertices().push_back(vertices[leftFaceIndices[faceIndex] * 5 + 1] + y);
                    mesh.GetVertices().push_back(vertices[leftFaceIndices[faceIndex] * 5 + 2] + z);

                    mesh.GetVertices().push_back(textureUVs[vertices[leftFaceIndices[faceIndex] * 5 + 3]]);
                    mesh.GetVertices().push_back(textureUVs[vertices[leftFaceIndices[faceIndex] * 5 + 4]]);
                }

                for(int i = 0; i < 6; i++){
                    mesh.GetIndices().push_back(baseVertex + i);
                }
            }

            if(z == width - 1 || BlockStorage::GetBlock(blocks[blockFront])->GetType() == BlockType::AIR){
                unsigned int baseVertex = mesh.GetVertices().size() / 5;
                //Need to loop over the indices in this case top face indices
                //Then need to add the vertices to the mesh with the position offset
                for(int faceIndex = 0; faceIndex < 6; faceIndex++){
                    mesh.GetVertices().push_back(vertices[frontFaceIndices[faceIndex] * 5] + x);
                    mesh.GetVertices().push_back(vertices[frontFaceIndices[faceIndex] * 5 + 1] + y);
                    mesh.GetVertices().push_back(vertices[frontFaceIndices[faceIndex] * 5 + 2] + z);


                    mesh.GetVertices().push_back(textureUVs[vertices[frontFaceIndices[faceIndex] * 5 + 3]]);
                    mesh.GetVertices().push_back(textureUVs[vertices[frontFaceIndices[faceIndex] * 5 + 4]]);
                }

                for(int i = 0; i < 6; i++){
                    mesh.GetIndices().push_back(baseVertex + i);
                }
            }

            if(z == 0 || BlockStorage::GetBlock(blocks[blockBack])->GetType() == BlockType::AIR){
                unsigned int baseVertex = mesh.GetVertices().size() / 5;
                //Need to loop over the indices in this case top face indices
                //Then need to add the vertices to the mesh with the position offset
                for(int faceIndex = 0; faceIndex < 6; faceIndex++){
                    mesh.GetVertices().push_back(vertices[backFaceIndices[faceIndex] * 5] + x);
                    mesh.GetVertices().push_back(vertices[backFaceIndices[faceIndex] * 5 + 1] + y);
                    mesh.GetVertices().push_back(vertices[backFaceIndices[faceIndex] * 5 + 2] + z);


                    mesh.GetVertices().push_back(textureUVs[vertices[backFaceIndices[faceIndex] * 5 + 3]]);
                    mesh.GetVertices().push_back(textureUVs[vertices[backFaceIndices[faceIndex] * 5 + 4]]);
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