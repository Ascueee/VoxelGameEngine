#include <TextureAtlas.hpp>


TextureAtlas::TextureAtlas(glm::ivec2 textureResolution, int textureSize) : Texture(){
    this->textureResolution = textureResolution;
    this->textureSize = textureSize;
}

//The textureIndex represents what texture it is in the atlas
//Need to get the texture position in the atlas using the ID
//those are the UVS normalized
glm::vec4 TextureAtlas::GetUV(int textureID){
    int textureColumns = textureResolution.x / textureSize;

    int textureX = textureID % textureColumns;
    int textureY = textureID / textureColumns;

    float u = (float)textureX / textureColumns;
    float v = (float)textureY / (textureResolution.y / textureSize);
    float u1 = u + (float)textureSize / textureResolution.x;
    float v1 = v + (float)textureSize / textureResolution.y;
    
    return glm::vec4(u,v,u1, v1);
}

