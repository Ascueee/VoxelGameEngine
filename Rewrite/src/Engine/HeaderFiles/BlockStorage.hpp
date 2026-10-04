#pragma once
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <Block.hpp>

NLOHMANN_JSON_SERIALIZE_ENUM(BlockType, {
    {BlockType::AIR, "Air"},
    {BlockType::SOLID, "Solid"},
    {BlockType::LIQUID, "Liquid"}
});

class BlockStorage{
    public:
        BlockStorage();
        static void Load();
        static Block* GetBlock(ushort id);
    private:
        static std::unordered_map<ushort, Block> blocks;
};