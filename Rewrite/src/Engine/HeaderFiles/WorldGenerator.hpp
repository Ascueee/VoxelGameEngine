#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include "Chunk.hpp"
#include <ChunkRenderer.hpp>
#include <Renderer.hpp>
#include <BiomeStorage.hpp>
#include <filesystem>

struct Vec3Hash {
    std::size_t operator()(const glm::vec3& v) const {
        std::size_t h1 = std::hash<float>{}(v.x);
        std::size_t h2 = std::hash<float>{}(v.y);
        std::size_t h3 = std::hash<float>{}(v.z);

        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

class WorldGenerator{
    public:
        WorldGenerator();
        static void Load(glm::vec3 pos);
        static void GenerateWorld(glm::vec3 pos);
        static void RenderWorld(glm::vec3 pos);
        static Chunk* GetChunk(glm::vec3 chunkpos);
    private:
        static std::unordered_map<glm::vec3, Chunk, Vec3Hash> chunks;
        static int renderDistance;
        static bool firstRun;
        static glm::vec3 lastPosition;
};