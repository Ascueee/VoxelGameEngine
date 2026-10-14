#pragma once
#include <iostream>
#include <Mesh.hpp>

class Entity{
    public:
        Entity(std::string name);
        virtual void Load();
        virtual void Update();
        virtual ~Entity() = default;
    private:
        std::string name;

};