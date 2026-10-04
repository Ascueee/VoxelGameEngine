#pragma once
#include <iostream>

class Biome{
    public:
        Biome(std::string name, float biomeTemp, float biomeWetness, int islandAndContinent, std::vector<int> blockPallet);
        std::string& GetName();
        int& GetIslandness();
        float GetTemp();
        float GetWetness();
        std::vector<int> GetBlockPallet();
    private:
        std::string name;
        int islandness; 
        float temp;
        float wetness;
        std::vector<int> blockPallet;
};