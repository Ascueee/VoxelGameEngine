#pragma once
#include <iostream>
#include <sol/sol.hpp>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <Entity.hpp>
#include <Transform.hpp>
#include <CameraController.hpp>
#include <glm/glm.hpp>

class ScriptStorage{
    public:
        ScriptStorage();
        static void Load();
        static sol::protected_function GetScript(std::string name);

    private:
        static sol::state luaReader;
        static std::unordered_map<std::string, sol::protected_function> scripts;
};