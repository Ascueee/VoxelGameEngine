#pragma once
#include <iostream>
#include <unordered_map>

class TreeStructure{
    public:
        TreeStructure();
        void SetName(std::string name);
        void SetWidth(int width);
        void SetHeight(int height);
        void SetWoodId(unsigned int blockId);
        void SetBlocks(std::unordered_map<std::string, ushort> blocks);
        std::string GetName();
        std::unordered_map<std::string, ushort> GetBlocks();
        int GetWidth();
        int GetHeight();
        int GetWoodBlockId();
    private:
        std::string name;
        int width;
        int height;
        int woodBlockId;
        std::unordered_map<std::string, ushort> blocks;
};