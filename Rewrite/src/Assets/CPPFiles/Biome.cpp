#include "Biome.hpp"

Biome::Biome(std::string name, float biomeTemp, float biomeWetness, int islandAndContinent,
             std::vector<int> blockPallet, std::unordered_map<std::string, int> decorationPallet){
    this->name = name;
    this->temp = biomeTemp;
    this->wetness = biomeWetness;
    this->islandness = islandAndContinent;
    this->blockPallet = blockPallet;
    this->decorationPallet = decorationPallet;
}

int& Biome::GetIslandness(){
    return islandness;
}

float Biome::GetTemp(){
    return temp;
}

float Biome::GetWetness(){
    return wetness;
}

std::vector<int> Biome::GetBlockPallet(){
    return blockPallet;
}

unsigned int Biome::GetDecorationBlock(float foliageNoise){

    unsigned int closestBlock = 0;
    float closestDistance = 2.0f; // how close the noise has to be to a key to spawn it

    for(const auto& entry : decorationPallet){
        float distance = std::abs(std::stof(entry.first) - foliageNoise);
        if(distance < closestDistance){
            closestDistance = distance;
            closestBlock = entry.second;
        }
    }
    return closestBlock;
}

std::string& Biome::GetName(){
    return name;
}