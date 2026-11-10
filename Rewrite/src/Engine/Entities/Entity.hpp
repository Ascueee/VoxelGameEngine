#pragma once
#include <iostream>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <glm/glm.hpp>
#include <Package.hpp>

class Entity{
    public:
        Entity();
        void SetName(std::string name);
        std::string GetName();
        template <typename T>
        void AddPackage();
        template <typename T>
        T* GetPackage();
        
        virtual void Load();
        virtual void Update();
        virtual ~Entity();
    protected:
        std::string name;
        std::unordered_map<std::type_index, Package*> packages;
};

#include "Entity.tpp"


