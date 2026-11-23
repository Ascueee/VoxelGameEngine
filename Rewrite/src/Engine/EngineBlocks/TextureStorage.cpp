#include <TextureStorage.hpp>
#include <stb_image.h>

std::unordered_map<std::string, Texture> TextureStorage::textures;
TextureStorage::TextureStorage(){}

void TextureStorage::Load(){
    std::string folderPath = "/Users/hayyan/Desktop/Repos/CPPEngine/Rewrite/src/Assets/JsonFiles/BaseTextures";
    for (const auto& entry : std::filesystem::directory_iterator(folderPath)){
        std::ifstream file(entry.path());

        if(!file.is_open()){
            std::cout << "[Texture Storage] Failed to open file \n";
            continue;
        }

        nlohmann::json data;
        file >> data;

        Texture texture = Texture();
        texture.SetName(data["name"].get<std::string>());
        texture.SetType(data["type"].get<TextureType>());

        glGenTextures(1, &texture.GetHandle());
        glBindTexture(GL_TEXTURE_2D, texture.GetHandle());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        int width, height, nrChannels;
        unsigned char *textureData = stbi_load(data["path"].get<std::string>().c_str(), &width, &height, &nrChannels, 0);

        if (textureData)
        {
            GLenum format = GL_RGB;
            switch (nrChannels)
            {
                case 1: format = GL_RED;  break;
                case 3: format = GL_RGB;  break;
                case 4: format = GL_RGBA; break;
                default:
                    std::cout << "Unsupported channel count: " << nrChannels << std::endl;
                    break;
            }

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }

        stbi_image_free(textureData);

        textures.emplace(data["name"].get<std::string>(), texture);

        std::cout << "[TextureAtlas Storage] Loading {" << data["name"].get<std::string>() << "} \n";
    }
}

Texture* TextureStorage::GetTexture(std::string name){
    if(textures.count(name) > 0){
        return &textures.at(name);
    }
    else{
        std::cout << "Couldnt Find Texture \n";
        return &textures.at("Error"); 
    }
}