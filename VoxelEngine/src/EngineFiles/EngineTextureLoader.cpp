#include "EngineTextureLoader.h"

std::unordered_map<std::string, Texture> EngineTextureLoader::textures;

EngineTextureLoader::EngineTextureLoader(){}

void EngineTextureLoader::Load(){
    std::string folderPath = "/Users/hayyan/Desktop/Repos/CPPEngine/VoxelEngine/src/Textures";

    for(const auto& entry : std::filesystem::directory_iterator(folderPath)){
        std::string texturePath = entry.path().string();
        std::string textureName = entry.path().stem();
        std::cout << "TEXTURE NAME: " << textureName << std::endl;
        Texture texture(textureName);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels,0);

        
        if(data){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else{
            std::cout << "Failed to load texture" << std::endl;
        }

        stbi_image_free(data);

        textures.emplace(textureName, texture);
    }
}

Texture* EngineTextureLoader::GetTexture(std::string textureName){
    return &textures[textureName];
}