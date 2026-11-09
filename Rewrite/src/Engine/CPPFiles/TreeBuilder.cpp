#include <TreeBuilder.hpp>
#include "WorldGenerator.hpp"

class WorldGenerator;

FastNoise TreeBuilder::structureNoise;
TreeBuilder::TreeBuilder(){}

void TreeBuilder::Load(){
    structureNoise.SetNoiseType(FastNoise::Perlin);
    structureNoise.SetSeed(87654);
    structureNoise.SetFrequency(3.0f);
}


//Need to remake this so that its not dependent on the chunkPosition paramater get the chunk from the world block positions
void TreeBuilder::InitializeStructure(float worldX, float worldY, float worldZ, glm::vec3 chunkPos){
    //std::cout << "[Tree Builder] is it working \n";
    //Get noise value
    float treeNoise = ((structureNoise.GetNoise(50 * worldX, 50 * worldZ)));
    worldX = worldX + treeNoise;
    worldZ = worldZ + treeNoise;

    glm::vec3 blockPos = glm::vec3(worldX, worldY, worldZ);

    //min distence need to be the width of the structure
    TreeStructure* structure = StructureStorage::GetStructure("Oak Tree");
    AABB structureAABB = AABB(blockPos, glm::vec3(12,40,12));

    //need to get the chunks that are rendered from world generator
    // to see if the structure collides with a chunk collider
    //if the structure collides with the chunk collider 
    //check if the structure collides with AABBs inside
    //if it does dont build
    //if it doesnt add the AABB to the chunk AABBs and BUILD TREE PROCEDURALLY
    bool canBuild = true;
    for(auto& chunk : WorldGenerator::GetChunks()){
        //std::cout << "insideAABBs: " << chunk.second.GetChunkAABB()->insideAABBs.size() << "\n";
        AABB* chunkAABB = &chunk.second.GetChunkAABB()->aabb;
        if(chunkAABB->IsColliding(&structureAABB)){
            for(auto& insideAABB : chunk.second.GetChunkAABB()->insideAABBs){
                if(insideAABB.IsColliding(&structureAABB)){
                    //std::cout << "[Tree Builder] Colliding with a structure in chunk\n";
                    canBuild = false;
                }
            }
        }
    }

    //if a structure can be built go through the structure json and build each block 
    if(canBuild){
        for(auto& chunk : WorldGenerator::GetChunks()){
            AABB& chunkAABB = chunk.second.GetChunkAABB()->aabb;
            if(chunkAABB.IsColliding(&structureAABB)){
                chunk.second.GetChunkAABB()->insideAABBs.push_back(structureAABB);
                AABB& back = chunk.second.GetChunkAABB()->insideAABBs.back();
                //DebugRenderer::LoadAABB(back.GetMesh());
            }
        }

        BuildTree(blockPos, structure->GetHeight(), structure);
    }
}

//this controls the procedural tree generation
//Needs to be a recursive function that builds each block
//TODO COMBINE BUILD BRANCH AND BUILD TREE TO ONE METHOD TO MAKE IT READABLE AND SIMPLER
//ALSO ADD BUILD CANOPY TO ADD LEAVES
void TreeBuilder::BuildTree(glm::vec3 blockPos, int trunkLength, TreeStructure* structure){

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> distrib(1, 8);
    int randomNum = distrib(gen);

    int width = 16;
    if(trunkLength < 0)
        return;
    
    //blockPos is in worldPosition need to convert it to a chunkPos
    //using chunkPos get chunk from the dictionary
    //then add the wood trunk block to the chunk
    
    glm::ivec3 currentChunk((int)floor(blockPos.x / 16.0f), 0, (int)floor(blockPos.z / 16.0f));
    glm::vec3 chunkOrigin = glm::vec3(currentChunk.x * 16, 0.0f, currentChunk.z * 16);
    glm::vec3 worldBlockPos = (blockPos + glm::vec3(0,1,0));
    glm::vec3 localBlockPos = worldBlockPos - chunkOrigin;

    int x = (int)localBlockPos.x;
    int y = (int)localBlockPos.y;
    int z = (int)localBlockPos.z;

    int blockIndex = x + (z * width) + ((y) * width * width);
    Chunk* chunk = WorldGenerator::GetChunk(chunkOrigin);
    if(chunk != nullptr)
        WorldGenerator::GetChunk(chunkOrigin)->GetBlocks()[blockIndex] = 10;
    
    //after placing the trunk block check to see if the trunk can branch
    TreeRules rules;
    if(trunkLength < 13 && trunkLength > 3){
        if(randomNum == 3){
            rules = TreeRules::GORIGHT;
        }
        else if(randomNum == 4){
            rules = TreeRules::GOLEFT;
        }
        else if(randomNum == 6){
            rules = TreeRules::GOFORWARD;
        }
        else if(randomNum == 7){
            rules = TreeRules::GOBACK;
        }

        BuildBranch(worldBlockPos, rules, structure->GetCanBranch(), structure->GetWidth(), structure);
    }

    trunkLength = trunkLength - 1;

    if(trunkLength == 0){
        BuildCanopy(worldBlockPos, structure);
    }

    BuildTree(worldBlockPos, trunkLength, structure);
}

void TreeBuilder::BuildBranch(glm::vec3 blockPos, TreeRules rules, bool canBuildBranch, int branchLength, TreeStructure* structure){

    if(branchLength < 0)
        return;


    int width = 16;
    glm::vec3 worldBlockPos;

    if(rules == TreeRules::GORIGHT){
        worldBlockPos = (blockPos + glm::vec3(1,0,0));   
        rules = TreeRules::GORIGHT;
    }
    else if(rules == TreeRules::GOLEFT){
        worldBlockPos = (blockPos + glm::vec3(-1,0,0));   
        rules = TreeRules::GOLEFT;
    }
    else if(rules == TreeRules::GOFORWARD){
        worldBlockPos = (blockPos + glm::vec3(0,0,1));
        rules = TreeRules::GOFORWARD;
    }
    else if(rules == TreeRules::GOBACK){
        worldBlockPos = (blockPos + glm::vec3(0,0,-1));
        rules = TreeRules::GOBACK;
    }
    else{
        worldBlockPos = (blockPos + glm::vec3(0,1,0));
        rules = TreeRules::GOUP;
    }
    
    glm::ivec3 currentChunk((int)floor(worldBlockPos.x / 16.0f), 0, (int)floor(worldBlockPos.z / 16.0f));
    glm::vec3 chunkOrigin = glm::vec3(currentChunk.x * 16, 0.0f, currentChunk.z * 16);
    glm::vec3 localBlockPos = worldBlockPos - chunkOrigin;

    int x = (int)localBlockPos.x;
    int y = (int)localBlockPos.y;
    int z = (int)localBlockPos.z;

    int blockIndex = x + (z * width) + ((y) * width * width);
    Chunk* chunk = WorldGenerator::GetChunk(chunkOrigin);
    if(chunk != nullptr)
        WorldGenerator::GetChunk(chunkOrigin)->GetBlocks()[blockIndex] = 10;
    branchLength = branchLength - 1;

    //if its the last branch build canopy of the tree
    //the canopy will be taken from the json
    //need to decide if the branch can go up and call BuildBranch in the up direction
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> distrib(1, 5);
    int randomNum = distrib(gen);
    
    std::uniform_int_distribution<int> distribTwo(1, 8);
    randomNum = distrib(gen);

    if(randomNum == 3 && canBuildBranch == true){
        canBuildBranch = false;
        //starts a new branch
        BuildBranch(worldBlockPos, TreeRules::GOUP, structure->GetWidth(), canBuildBranch, structure);
    }

    if(branchLength == 0){
        //std::cout << "[Tree Builder] building canopy \n";
        BuildCanopy(worldBlockPos, structure);
    }
    
    BuildBranch(worldBlockPos, rules, canBuildBranch, branchLength, structure);
}

//using the tree json file build the canopy based on the block pattern saved
//the canopys origin block will be placed on top of the last trunk block or branch block
//TODO:: MAKE THIS USE SHAPES AGMORITHIMCLY, SINE FOR (wimping willow like trees), spheres, squares)
void TreeBuilder::BuildCanopy(glm::vec3 rootPos, TreeStructure* structure){
    //go through all the structure blocks and add offsets to the root block and add it to the chunk
    //the root block will be used to add all of the offsets to it which will give the location in the chunk to place the block
    for(auto& [offSet, blockId] : structure->GetBlocks()){
        std::stringstream ss(offSet);

        int offSetX, offSetY, offSetZ;
        char comma1, comma2;
        ss >> offSetX >> comma1 >> offSetY >> comma2 >> offSetZ;

        //now place the block
        glm::vec3 worldBlockPos = rootPos + glm::vec3(offSetX, offSetY, offSetZ);
        glm::ivec3 currentChunk((int)floor(worldBlockPos.x / 16.0f), 0, (int)floor(worldBlockPos.z / 16.0f));
        glm::vec3 chunkOrigin = glm::vec3(currentChunk.x * 16, 0.0f, currentChunk.z * 16);
        glm::vec3 localBlockPos = worldBlockPos - chunkOrigin;
        int width = 16;

        int x = (int)localBlockPos.x;
        int y = (int)localBlockPos.y;
        int z = (int)localBlockPos.z;

        int blockIndex = x + (z * width) + ((y) * width * width);
        Chunk* chunk = WorldGenerator::GetChunk(chunkOrigin);
        if(chunk != nullptr)
            WorldGenerator::GetChunk(chunkOrigin)->GetBlocks()[blockIndex] = structure->GetLeavesId();
    }
}
