#pragma once
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <TreeStructure.hpp>
#include <StructureStorage.hpp>
#include <nlohmann/json.hpp>

class StructureStorage{
    public:
        StructureStorage();
        static void Load();
        static TreeStructure* GetStructure(std::string name);
    private:
        static std::unordered_map<std::string, TreeStructure> structures;

};