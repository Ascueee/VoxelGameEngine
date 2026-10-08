#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <functional>
//Blocks are what make up the terrain
// id: represents what type of block it is(ex: 0 = air, 1 = Dirt)
// type: determines the properties of the block(air - no collisions, solid - collisions)
enum BlockType{
    AIR,
    SOLID,
    LIQUID
};

enum BlockFace{
    TOP,
    BOTTOM,
    RIGHT,
    LEFT,
    FRONT,
    BACK
};

class Block{
    public:
        Block(std::string name, unsigned int id, std::vector<unsigned int> textureIds, BlockType type);
        void SetPosition(glm::vec3 position);
        void SetId(unsigned int id);
        std::string& GetName();
        unsigned int& GetId();
        unsigned int& GetTextureId(BlockFace face);
        glm::vec3 GetPosition();
        BlockType& GetType();
    private:
        std::string name;
        unsigned int id;
        std::vector<unsigned int> textureIds;
        BlockType type;
};