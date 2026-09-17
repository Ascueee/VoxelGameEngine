#include "ChunkSystem.h"

void ChunkSystem::BuildChunk(Entity* ent){
    ChunkComponent& chunkComponent = StorageManager::GetStorage()->chunkStorage[ent->GetID()];
    MeshComponent& meshComponent = StorageManager::GetStorage()->meshStorage[ent->GetID()];

    BuildGrid(chunkComponent);
    BuildMesh(chunkComponent,meshComponent);
}

//This will add the base ground layer of the chunk
void ChunkSystem::BuildGrid(ChunkComponent& chunk){
    for(int x = 0; x < chunk.width; x++){
        for(int y = 0; y < chunk.height; y++){
            for(int z = 0; z < chunk.width; z++){
                Block block;

                if(y < 5)
                    block.blockID = 1;
                else
                    block.blockID = 0;

                chunk.blocks[x][y][z] = block;
            }
        }
    }
}

//Will take the chunkComponents block Grid and will build a mesh where there is no air and for the specific block model
void ChunkSystem::BuildMesh(ChunkComponent& chunk, MeshComponent& mesh){
    for(int x = 0; x < chunk.width; x++){
        for(int y = 0; y < chunk.height; y++){
            for(int z = 0; z < chunk.width; z++){
                
                //Builds the mesh by checking if the sides of the cube is either covered by a block or air
                //if its air add to the mesh
                if(!IsBlockSolid(chunk.blocks[x][y][z]))
                    continue;
                
                //Checks if the block above the current block is solid
                //if its not solid build the top part of the mesh
                if(y + 1 >= chunk.height || !IsBlockSolid(chunk.blocks[x][y + 1][z])){
                    GenerateCubeMesh(mesh, BlockDirection::BLOCKTOP, glm::vec3(x,y,z));
                }

                if(y - 1 < 0 || !IsBlockSolid(chunk.blocks[x][y - 1][z])){
                    GenerateCubeMesh(mesh, BlockDirection::BLOCKBOTTOM, glm::vec3(x,y,z));
                }

                if(x + 1 >= chunk.width || !IsBlockSolid(chunk.blocks[x + 1][y][z])){
                    if(chunk.chunkNeighbours[2] == -1)
                        continue;
                    ChunkComponent& rightNeighbour = StorageManager::GetStorage()->chunkStorage[chunk.chunkNeighbours[2]];
                    //Need to check the neighbouring chunk to see if there is a block dont render same for left, front, back
                    if(!IsBlockSolid(rightNeighbour.blocks[0][y][z])){
                        GenerateCubeMesh(mesh, BlockDirection::BLOCKRIGHT, glm::vec3(x,y,z));
                    }
                }

                if(x - 1 < 0 || !IsBlockSolid(chunk.blocks[x - 1][y][z])){
                    if(chunk.chunkNeighbours[3] == -1)
                        continue;
                    ChunkComponent& leftNeighbour = StorageManager::GetStorage()->chunkStorage[chunk.chunkNeighbours[3]];
                    if(!IsBlockSolid(leftNeighbour.blocks[15][y][z])){
                        GenerateCubeMesh(mesh, BlockDirection::BLOCKLEFT, glm::vec3(x,y,z));
                    }
                }

                if(z + 1 >= chunk.width || !IsBlockSolid(chunk.blocks[x][y][z + 1])){
                    if(chunk.chunkNeighbours[0] == -1)
                        continue;
                    ChunkComponent& frontNeighbour = StorageManager::GetStorage()->chunkStorage[chunk.chunkNeighbours[0]];
                    if(!IsBlockSolid(frontNeighbour.blocks[x][y][0])){
                        GenerateCubeMesh(mesh, BlockDirection::BLOCKFRONT, glm::vec3(x,y,z));
                    }
                }

                if(z - 1 < 0 || !IsBlockSolid(chunk.blocks[x][y][z - 1])){
                    if(chunk.chunkNeighbours[1] == -1)
                        continue;
                    ChunkComponent& backNeighbour = StorageManager::GetStorage()->chunkStorage[chunk.chunkNeighbours[1]];
                    if(!IsBlockSolid(backNeighbour.blocks[x][y][15])){
                        GenerateCubeMesh(mesh, BlockDirection::BLOCKBACK, glm::vec3(x,y,z));
                    }
                }
            }
        }
    }
}

void ChunkSystem::GenerateCubeMesh(MeshComponent& mesh, BlockDirection dir, glm::vec3 blockPos){
    float cubeVertices[] = {
        0.5f, 0.5f, 0.5f,//On vertex which is 0
        -0.5f, 0.5f, 0.5f, //indice 1
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f
    };

    unsigned int topFace[] = {
        0,5,4,4,1,0
    };
    unsigned int bottomFace[] = {
        3,2,7,7,6,3
    };

    unsigned int frontFace[] = {
        0,1,2,2,3,0
    };

    unsigned int backFace[] = {
        5,4,7,7,6,5
    };

    unsigned int rightFace[] = {
        0,3,6,6,5,0
    };

    unsigned int leftFace[] = {
        1,4,7,7,2,1
    };

    float topFaceUV[] = {0.0f,0.0f, 1.0f,0.0f, 1.0f,1.0f, 1.0f,1.0f, 0.0f,1.0f, 0.0f,0.0f};
    float bottomFaceUV[] = {0.0f,0.0f, 1.0f,0.0f, 1.0f,1.0f, 1.0f,1.0f, 0.0f,1.0f, 0.0f,0.0f};
    float frontFaceUV[] = {0.0f,0.0f, 1.0f,0.0f, 1.0f,1.0f, 1.0f,1.0f, 0.0f,1.0f, 0.0f,0.0f};
    float backFaceUV[] = {0.0f,0.0f, 1.0f,0.0f, 1.0f,1.0f, 1.0f,1.0f, 0.0f,1.0f, 0.0f,0.0f};
    float leftFaceUV[] = {0.0f,0.0f, 1.0f,0.0f, 1.0f,1.0f, 1.0f,1.0f, 0.0f,1.0f, 0.0f,0.0f};
    float rightFaceUV[] = {0.0f,0.0f, 1.0f,0.0f, 1.0f,1.0f, 1.0f,1.0f, 0.0f,1.0f, 0.0f,0.0f};
    unsigned int baseVertex = mesh.mesh.vertices.size() / 5;

    for(int i = 0; i < 6; i++){
        //Needs to loop over the top face get the indices and add it to the chunk mesh
        //This is for the top face
        //Adds one vertex depending on the indices from the face
        if(dir == BlockDirection::BLOCKTOP){
            mesh.mesh.vertices.push_back(cubeVertices[topFace[i] * 3] + blockPos.x);
            mesh.mesh.vertices.push_back(cubeVertices[topFace[i] * 3 + 1] + blockPos.y);
            mesh.mesh.vertices.push_back(cubeVertices[topFace[i] * 3 + 2] + blockPos.z);

            //need to add uvs
            mesh.mesh.vertices.push_back(topFaceUV[i * 2]);
            mesh.mesh.vertices.push_back(topFaceUV[i * 2 + 1]);
        }
        else if(dir == BlockDirection::BLOCKBOTTOM){
            mesh.mesh.vertices.push_back(cubeVertices[bottomFace[i] * 3] + blockPos.x);
            mesh.mesh.vertices.push_back(cubeVertices[bottomFace[i] * 3 + 1] + blockPos.y);
            mesh.mesh.vertices.push_back(cubeVertices[bottomFace[i] * 3 + 2] + blockPos.z);

            //need to add uvs
            mesh.mesh.vertices.push_back(bottomFaceUV[i * 2]);
            mesh.mesh.vertices.push_back(bottomFaceUV[i * 2 + 1]);
        }
        else if(dir == BlockDirection::BLOCKRIGHT){
            mesh.mesh.vertices.push_back(cubeVertices[rightFace[i] * 3] + blockPos.x);
            mesh.mesh.vertices.push_back(cubeVertices[rightFace[i] * 3 + 1] + blockPos.y);
            mesh.mesh.vertices.push_back(cubeVertices[rightFace[i] * 3 + 2] + blockPos.z);

            //need to add uvs
            mesh.mesh.vertices.push_back(rightFaceUV[i * 2]);
            mesh.mesh.vertices.push_back(rightFaceUV[i * 2 + 1]);
        }
        else if(dir == BlockDirection::BLOCKLEFT){
            mesh.mesh.vertices.push_back(cubeVertices[leftFace[i] * 3] + blockPos.x);
            mesh.mesh.vertices.push_back(cubeVertices[leftFace[i] * 3 + 1] + blockPos.y);
            mesh.mesh.vertices.push_back(cubeVertices[leftFace[i] * 3 + 2] + blockPos.z);

            //need to add uvs
            mesh.mesh.vertices.push_back(leftFaceUV[i * 2]);
            mesh.mesh.vertices.push_back(leftFaceUV[i * 2 + 1]);
        }
        else if(dir == BlockDirection::BLOCKFRONT){
            mesh.mesh.vertices.push_back(cubeVertices[frontFace[i] * 3] + blockPos.x);
            mesh.mesh.vertices.push_back(cubeVertices[frontFace[i] * 3 + 1] + blockPos.y);
            mesh.mesh.vertices.push_back(cubeVertices[frontFace[i] * 3 + 2] + blockPos.z);

            //need to add uvs
            mesh.mesh.vertices.push_back(frontFaceUV[i * 2]);
            mesh.mesh.vertices.push_back(frontFaceUV[i * 2 + 1]);
        }
        else if(dir == BlockDirection::BLOCKBACK){
            mesh.mesh.vertices.push_back(cubeVertices[backFace[i] * 3] + blockPos.x);
            mesh.mesh.vertices.push_back(cubeVertices[backFace[i] * 3 + 1] + blockPos.y);
            mesh.mesh.vertices.push_back(cubeVertices[backFace[i] * 3 + 2] + blockPos.z);

            //need to add uvs
            mesh.mesh.vertices.push_back(backFaceUV[i * 2]);
            mesh.mesh.vertices.push_back(backFaceUV[i * 2 + 1]);
        }
    }

    for(int i = 0; i < 6; i++){
        mesh.mesh.indices.push_back(baseVertex + i);
    }
}

bool ChunkSystem::IsBlockSolid(Block block){
    if(block.blockID == 0)
        return false;
    else
        return true;
}
