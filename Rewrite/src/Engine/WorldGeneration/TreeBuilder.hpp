#pragma once
#include <FastNoise.h>
#include <iostream>
#include <unordered_map>
#include <TreeStructure.hpp>
#include <StructureStorage.hpp>
#include <DebugRenderer.hpp>
#include <Vector3Hash.hpp>
#include <random>
#include <sstream>



enum TreeRules{
    GOLEFT,
    GORIGHT,
    GOFORWARD,
    GOBACK,
    GOUP,
    BRANCH,
    LEAF
};

class TreeBuilder{
    public:
        TreeBuilder();
        static void Load();
        static void InitializeStructure(float worldX, float worldY, float worldZ, glm::vec3 chunkPos);
    private:
        static FastNoise structureNoise;
        //Key a structure position is the key
        static void BuildTree(glm::vec3 blockPos, int trunkLength, TreeStructure* structure);
        static void BuildBranch(glm::vec3 blockPos, TreeRules rules, bool canBuildBranch, int branchLength, TreeStructure* structure);
        static void BuildCanopy(glm::vec3 rootPos, TreeStructure* structure);
};