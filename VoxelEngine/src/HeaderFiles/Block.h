#pragma once

//Blocks are what interface with the world grid
//They can be placed in the world/destroyed
//Can have different models for blocks so not just cubes
//Need to also represent foliage/decorations around the world
//Need to also represent liquid like water/lava etc
struct Block{
    unsigned int blockID;
};