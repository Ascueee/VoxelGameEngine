#pragma once
#include <iostream>
#include <sol/sol.hpp>
#include <fstream>
#include <filesystem>
#include <Script.hpp>
#include <unordered_map>
#include <Entity.hpp>
#include <Transform.hpp>
#include <CameraController.hpp>
#include <EntityStorage.hpp>
#include <Mesh.hpp>
#include <ModelController.hpp>
#include <ModelStorage.hpp>
#include <glm/glm.hpp>

class ScriptStorage{
    public:
        ScriptStorage();
        static void Load();
        static Script* GetScript(std::string name);

    private:
        static sol::state luaReader;
        static std::unordered_map<std::string, Script> scripts;
};