#include <BiomeStorage.hpp>

std::unordered_map<glm::vec2, Biome, Vec2Hash>  BiomeStorage::biomes;
BiomeStorage::BiomeStorage(){}

void BiomeStorage::Load(){
    std::string folderPath = "/Users/hayyan/Desktop/Repos/CPPEngine/Rewrite/src/Assets/Biomes";

   for (const auto& entry : std::filesystem::directory_iterator(folderPath)){
        std::ifstream file(entry.path());

        if(!file.is_open()){
            std::cout << "[Biome Storage] Failed to open file \n";
            continue;
        }

        nlohmann::json data;
        file >> data;

        Biome biome = Biome(data["name"].get<std::string>(), data["WorldGeneration"]["biomeTemp"].get<float>(),
         data["WorldGeneration"]["biomeWetness"].get<float>(), data["WorldGeneration"]["islandness"].get<int>(),
          data["TerrainBlocks"]["blockPallet"].get<std::vector<int>>(), data["Decoration"]["foliageBlocks"].get<std::unordered_map<std::string, int>>());
        biomes.emplace(glm::vec2(data["WorldGeneration"]["biomeTemp"].get<float>(),data["WorldGeneration"]["biomeWetness"].get<float>()), biome);
        std::cout << "[Biome Storage] Loading Biome: " << data["name"].get<std::string>() << ", " 
        << data["WorldGeneration"]["islandness"].get<int>() << "\n";
   }
}

Biome* BiomeStorage::GetBiomeByName(std::string name){
    for (auto& biome : biomes)
    {
        if (biome.second.GetName() == name)
        {
            return &biome.second;
        }
    }

    return nullptr;
}

Biome* BiomeStorage::GetBiomeByTempAndWetness(glm::vec2 tempAndWetness)
{
    Biome* closestBiome = nullptr;
    float closestDistance = FLT_MAX;

    for (auto& biome : biomes)
    {
        glm::vec2 currentTempAndWetness(biome.second.GetTemp(), biome.second.GetWetness());

        float distance = glm::dot(currentTempAndWetness - tempAndWetness, currentTempAndWetness - tempAndWetness);

        if (distance < closestDistance)
        {
            closestDistance = distance;
            closestBiome = &biome.second;
        }
    }

    return closestBiome;
}