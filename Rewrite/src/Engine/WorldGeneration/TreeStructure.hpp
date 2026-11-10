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
        void SetLeavesId(unsigned int blockId);
        void SetCanBranch(bool canBranch);
        void SetBlocks(std::unordered_map<std::string, ushort> blocks);
        std::string GetName();
        std::unordered_map<std::string, ushort> GetBlocks();
        int GetWidth();
        int GetHeight();
        ushort GetWoodBlockId();
        ushort GetLeavesId();
        bool GetCanBranch();
    private:
        std::string name;
        int width;
        int height;
        ushort woodBlockId;
        ushort leavesBlockId;
        bool canBranch;
        std::unordered_map<std::string, ushort> blocks;
};