#pragma once
#include <fstream>
#include <filesystem>
#include <Biome.hpp>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <glm/glm.hpp>

struct Vec2Hash {
    std::size_t operator()(const glm::vec2& v) const {
        std::size_t h1 = std::hash<float>{}(v.x);
        std::size_t h2 = std::hash<float>{}(v.y);

        return h1 ^ (h2 << 1);
    }
};

class BiomeStorage{
    public:
        BiomeStorage();
        static void Load();
        static Biome* GetBiomeByName(std::string name);
        static Biome* GetBiomeByTempAndWetness(glm::vec2 tempAndWetness);
    private:
        static std::unordered_map<glm::vec2, Biome, Vec2Hash> biomes;
};