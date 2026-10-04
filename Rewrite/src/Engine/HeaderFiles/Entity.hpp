#pragma once
#include <iostream>

class Entity{
    public:
        Entity(std::string name);
        std::string name;
        virtual void Load();
        virtual void Update();
        virtual ~Entity() = default;
    private:
};