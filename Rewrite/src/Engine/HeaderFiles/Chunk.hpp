#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <Block.hpp>
#include <Mesh.hpp>
#include <Biome.hpp>
#include <TextureAtlasStorage.hpp>
#include <BlockStorage.hpp>
#include <BiomeStorage.hpp>
#include <BlockModelStorage.hpp>
#include <fastnoise/FastNoise.h>
#include <unordered_map>
#include <functional>
#include <BlockEvents.hpp>
#include <fstream>
#include <filesystem>
#include <sstream>

//A chunk is a portion of terain which holds blocks
// chunkPosition - where the chunk is in the world
// blocks - all the blocks in the terrain in a 1D vector
// chunkMesh - The mesh that will render in the game world
class Chunk{
    public:
        Chunk();
        Chunk(glm::vec3 position);
        void BuildChunk(glm::vec3 pos);
        void BuildChunkMesh(glm::vec3 pos);
        void BuildMesh(Mesh& mesh, BlockMesh* blockMesh, int block, glm::vec3 blockPos, BlockFace blockFace);
        void UpdateChunk(glm::vec3 pos);
        void SubscribeBlockEvent(int blockIndex, std::function<void(Chunk&, int)> event);
        void PlaceBlock(glm::vec3 blockPos, unsigned int id);
        void SetIsGenerated(bool isGenerated);
        void SaveChunk();
        void LoadChunk(std::ifstream& file);
        Mesh& GetTerrainMesh();
        std::vector<ushort>& GetBlocks();
        glm::vec3& GetPosition();
        bool GetIsGenerated();
    private:
        unsigned int width = 16;
        unsigned int height = 256;
        glm::vec3 position;
        std::vector<ushort> blocks;
        //Chunk stores block events per each block in the chunk by their id in the blocks array
        //Then it saves the blocks index in the blocks and also all the events the block has
        std::unordered_map<int, std::function<void(Chunk&, int)>> blockEvents;
        Mesh terrainMesh;
        bool isGenerated = false;
        FastNoise islandness;
        FastNoise mountainess;
        FastNoise tempreture;
        FastNoise wetness;
        FastNoise foliage;
        Biome* biome;
};