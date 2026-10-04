#pragma once
#include <iostream>
#include <Chunk.hpp>
#include <glad/glad.h>
class Chunk;
class BlockEvents{
    public:
        static void TestEvent(Chunk& chunk, int blockIndex); 
        static void LiquidBlock(Chunk& chunk, int blockIndex);
};