#pragma once
#include <iostream>
#include <unordered_map>
#include <glm/glm.hpp>
#include <string> 

class Biome{
    public:
        Biome(std::string name, float biomeTemp, float biomeWetness, int islandAndContinent,
             std::vector<int> blockPallet, std::unordered_map<std::string, int> decorationPallet);
        std::string& GetName();
        int& GetIslandness();
        float GetTemp();
        float GetWetness();
        std::vector<int> GetBlockPallet();
        unsigned int GetDecorationBlock(float foliageNoise);
    private:
        std::string name;
        int islandness; 
        float temp;
        float wetness;
        std::vector<int> blockPallet;
        std::unordered_map<std::string, int> decorationPallet;
};