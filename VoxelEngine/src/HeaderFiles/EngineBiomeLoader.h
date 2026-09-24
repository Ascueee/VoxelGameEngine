#pragma once
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include "nlohmann/json.hpp"
#include <fstream>


struct Biome{
    std::string name;
    float mountainess;
    float islandsAndContinents;
    float errosian;

};

class EngineBiomeLoader{
    public:
        EngineBiomeLoader();
        static void Load();
        static Biome* GetBiome(std::string name);
    private:
        static std::unordered_map<std::string, Biome> biomes;


};