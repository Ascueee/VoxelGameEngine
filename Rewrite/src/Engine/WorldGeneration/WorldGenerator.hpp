#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include "Chunk.hpp"
#include <Renderer.hpp>
#include <BiomeStorage.hpp>
#include <filesystem>
#include <DebugRenderer.hpp>
#include <Vector3Hash.hpp>

struct BroadPhaseCell{
    glm::vec3 position;
    glm::vec3 rootPosition;
    AABB cellAABB;
    std::vector<AABB> insideAABBs;
};

class WorldGenerator{
    public:
        WorldGenerator();
        static void Load(glm::vec3 pos);
        static void GenerateWorld(glm::vec3 pos);
        static void GeometryPass(glm::vec3 pos, int fbWidth, int fbHeight);
        static void ForwardPass();
        static void CreateChunk(glm::vec3 chunkPosition);
        static void LoadChunk(glm::vec3 chunkPosition);
        static Chunk* GetChunk(glm::vec3 chunkpos);
        static std::unordered_map<glm::vec3, Chunk, Vec3Hash>& GetChunks();
    private:
        static std::unordered_map<glm::vec3, Chunk, Vec3Hash> chunks;
        static int renderDistance;
        static bool firstRun;
        static glm::vec3 lastPlayerPosition;
};