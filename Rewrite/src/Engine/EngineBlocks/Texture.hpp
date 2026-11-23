#pragma once
#include <iostream>
#include <nlohmann/json.hpp>


enum TextureType{
    BASE,
    ATLAS
};

NLOHMANN_JSON_SERIALIZE_ENUM(TextureType, {
    {TextureType::BASE, "Base"},
    {TextureType::ATLAS, "Atlas"},
});

class Texture{
    public:
        Texture();
        void SetName(std::string name);
        void SetType(TextureType type);
        unsigned int& GetHandle();
        TextureType GetType();
        std::string GetName();
    protected:
        std::string name;
        unsigned int handle;
        TextureType type;
};