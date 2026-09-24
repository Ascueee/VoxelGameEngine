#include "EngineBiomeLoader.h"

using json = nlohmann::json;

std::unordered_map<std::string, Biome> EngineBiomeLoader::biomes;
EngineBiomeLoader::EngineBiomeLoader(){}

void EngineBiomeLoader::Load(){
    std::string folderPath = "/Users/hayyan/Desktop/Repos/CPPEngine/VoxelEngine/src/WorldGen/Biomes";

    for(const auto& entry : std::filesystem::directory_iterator(folderPath)){

        if (entry.path().extension() != ".json")
            continue;
        

        std::ifstream file(entry.path());

        json data;
        file >> data;

        Biome biome;
        biome.name = data["name"].get<std::string>();
        biome.mountainess = data["mountainess"].get<float>();
        biome.islandsAndContinents = data["islandsAndContinents"].get<float>();
        biome.errosian = data["errosian"].get<float>();

        biomes.emplace(biome.name, biome);

        std::cout << "Added Biome{" << biome.name << "}" << std::endl;
    }
}


Biome* EngineBiomeLoader::GetBiome(std::string name){
    return &biomes[name];
}