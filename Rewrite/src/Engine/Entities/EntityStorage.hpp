#pragma once
#include <iostream>
#include <unordered_map>
#include <Entity.hpp>
#include <Renderer.hpp>
#include <ScriptStorage.hpp>

#include <Transform.hpp>
#include <CameraController.hpp>
#include <ModelController.hpp>
#include <ScriptController.hpp>

//This class will keep all the entities in the engine
//the lua script will be able to access this class to retrieve specific entities
class EntityStorage{
    public:
        EntityStorage();
        static void Load();
        static void LoadEntities();
        static void UpdateEntities();
        static void GeometryPass();
        static void ForwardPass();
        static void CreateEntity(std::string name);
        static Entity* GetEntity(std::string name);
    private:
        static std::unordered_map<std::string, Entity> entities;

};