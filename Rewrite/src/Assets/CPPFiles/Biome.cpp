#include "Biome.hpp"

Biome::Biome(std::string name, float biomeTemp, float biomeWetness, int islandAndContinent, std::vector<int> blockPallet){
    this->name = name;
    this->temp = biomeTemp;
    this->wetness = biomeWetness;
    this->islandness = islandAndContinent;
    this->blockPallet = blockPallet;
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
std::string& Biome::GetName(){
    return name;
}